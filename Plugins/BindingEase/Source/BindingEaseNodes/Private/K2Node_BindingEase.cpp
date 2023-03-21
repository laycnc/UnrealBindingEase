// Copyright Epic Games, Inc. All Rights Reserved.

#include "K2Node_BindingEase.h"
#include "Containers/Array.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "Engine/MemberReference.h"
#include "EngineLogs.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformCrt.h"
#include "HAL/PlatformMath.h"
#include "Internationalization/Internationalization.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/CompilerResultsLog.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "Logging/MessageLog.h"
#include "Misc/AssertionMacros.h"
#include "Serialization/Archive.h"
#include "Templates/Casts.h"
#include "Templates/SubclassOf.h"
#include "Trace/Detail/Channel.h"
#include "UObject/Class.h"
#include "UObject/Field.h"
#include "UObject/Object.h"
#include "UObject/ObjectVersion.h"
#include "UObject/UnrealType.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"

#include "Engine/DynamicBlueprintBinding.h"
#include "SimplePropertyDelegateBinding.h"

#define LOCTEXT_NAMESPACE "K2Node"

static TAutoConsoleVariable<bool> CVarBPEnableDeprecatedWarningForComponentDelegateNodes(
	TEXT("bp.EnableDeprecatedWarningForComponentDelegateNodes"),
	true,
	TEXT("Show Deprecated warning for component delegate event nodes"),
	ECVF_Cheat);

// @TODO_BH: Remove the CVar for validity checking when we can get all the errors sorted out
namespace PinValidityCheck
{
	/**
	* CVar controls pin validity warning which will throw when a macro graph is silently failing
	* @see UE-100024
	*/
	static bool bDisplayMissingBoundComponentWarning = true;
	static FAutoConsoleVariableRef CVarDisplayMissingBoundComponentWarning(
		TEXT("bp.PinValidityCheck.bDisplayMissingBoundComponentWarning"), bDisplayMissingBoundComponentWarning,
		TEXT("CVar controls pin validity warning which will throw when a bound event has no matching component"),
		ECVF_Default);
}

UK2Node_BindingEase::UK2Node_BindingEase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UK2Node_BindingEase::Modify(bool bAlwaysMarkDirty)
{
	CachedNodeTitle.MarkDirty();

	return Super::Modify(bAlwaysMarkDirty);
}

bool UK2Node_BindingEase::CanPasteHere(const UEdGraph* TargetGraph) const
{
	// By default, to be safe, we don't allow events to be pasted, except under special circumstances (see below)
	bool bDisallowPaste = !Super::CanPasteHere(TargetGraph);
	if (!bDisallowPaste)
	{
#if 0 // todo
		UBlueprint* BlueprintForGraph = FBlueprintEditorUtils::FindBlueprintForGraph(TargetGraph);
		if (const UK2Node_Event* PreExistingNode = FKismetEditorUtilities::FindBoundEventForComponent(BlueprintForGraph, DelegatePropertyName, ComponentPropertyName))
		{
			//UE_LOG(LogBlueprint, Log, TEXT("Cannot paste event node (%s) directly because it is flagged as an internal event."), *GetFName().ToString());
			bDisallowPaste = true;
		}
#endif
	}
	return !bDisallowPaste;

}
FText UK2Node_BindingEase::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (CachedNodeTitle.IsOutOfDate(this))
	{

		if (DelegateOwnerClass.IsNull() || DelegatePropertyName == NAME_None)
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("DelegatePropertyName"), GetTargetDelegateDisplayName());
			Args.Add(TEXT("DelegateOwnerClassName"), FText::FromString(GetNameSafe(DelegateOwnerClass)));

			// FText::Format() is slow, so we cache this to save on performance
			CachedNodeTitle.SetCachedText(LOCTEXT("ComponentBoundEvent_DefaultTitle", "Binding Ease"), this);
		}
		else
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("DelegatePropertyName"), GetTargetDelegateDisplayName());
			Args.Add(TEXT("DelegateOwnerClassName"), FText::FromString(GetNameSafe(DelegateOwnerClass)));

			// FText::Format() is slow, so we cache this to save on performance
			CachedNodeTitle.SetCachedText(FText::Format(LOCTEXT("ComponentBoundEvent_Title", "{DelegatePropertyName} ({DelegateOwnerClassName}) by Binding Ease"), Args), this);
		}
	}
	return CachedNodeTitle;
}

FText UK2Node_BindingEase::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);
}

void UK2Node_BindingEase::InitializeComponentBoundEventParams(const FMulticastDelegateProperty* InDelegateProperty)
{
	if (InDelegateProperty)
	{
		DelegatePropertyName = InDelegateProperty->GetFName();
		DelegateOwnerClass = CastChecked<UClass>(InDelegateProperty->GetOwner<UObject>())->GetAuthoritativeClass();

		EventReference.SetFromField<UFunction>(InDelegateProperty->SignatureFunction, /*bIsConsideredSelfContext =*/false);

		CustomFunctionName = FName(*FString::Printf(TEXT("BndEvt__%s_%s_%s"), *GetBlueprint()->GetName(), *GetName(), *EventReference.GetMemberName().ToString()));
		bOverrideFunction = false;
		bInternalEvent = true;
		CachedNodeTitle.MarkDirty();
	}
}

UClass* UK2Node_BindingEase::GetDynamicBindingClass() const
{
	return USimplePropertyDelegateBinding::StaticClass();
}

void UK2Node_BindingEase::RegisterDynamicBinding(UDynamicBlueprintBinding* BindingObject) const
{
	USimplePropertyDelegateBinding* ComponentBindingObject = CastChecked<USimplePropertyDelegateBinding>(BindingObject);

	FBlueprintSimplePropertyDelegateBinding Binding;
	Binding.DelegateOwnerClass = DelegateOwnerClass;
	Binding.DelegatePropertyName = DelegatePropertyName;
	Binding.FunctionNameToBind = CustomFunctionName;

	CachedNodeTitle.MarkDirty();
	ComponentBindingObject->DelegateBindings.Add(Binding);
}

void UK2Node_BindingEase::HandleVariableRenamed(UBlueprint* InBlueprint, UClass* InVariableClass, UEdGraph* InGraph, const FName& InOldVarName, const FName& InNewVarName)
{
	if (InVariableClass && InVariableClass->IsChildOf(InBlueprint->GeneratedClass))
	{
#if 0 // todo
		// This could be the case if the component that this was originally bound to was removed, and a new one was 
		// added in it's place. @see UE-88511
		if (InNewVarName == ComponentPropertyName)
		{
			FCompilerResultsLog LogResults;
			FMessageLog MessageLog("BlueprintLog");
			LogResults.Error(*LOCTEXT("ComponentBoundEvent_Rename_Error", "There can only be one event node bound to this component! Delete @@ or the other bound event").ToString(), this);

			MessageLog.NewPage(LOCTEXT("ComponentBoundEvent_Rename_Error_Label", "Rename Component Error"));
			MessageLog.AddMessages(LogResults.Messages);
			MessageLog.Notify(LOCTEXT("OnConvertEventToFunctionErrorMsg", "Renaming a component"));
			FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(this);
		}
		else if (InOldVarName == ComponentPropertyName)
		{
			Modify();
			ComponentPropertyName = InNewVarName;
		}
#endif
	}
}

void UK2Node_BindingEase::ValidateNodeDuringCompilation(FCompilerResultsLog& MessageLog) const
{
	if (PinValidityCheck::bDisplayMissingBoundComponentWarning && !IsDelegateValid())
	{
		MessageLog.Warning(*LOCTEXT("ComponentBoundEvent_Error", "@@ does not have a valid matching component!").ToString(), this);
	}
	Super::ValidateNodeDuringCompilation(MessageLog);
}

void UK2Node_BindingEase::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

bool UK2Node_BindingEase::IsDelegateValid() const
{
	const UBlueprint* const BP = GetBlueprint();
	// Validate that the property has not been renamed or deleted via the SCS tree
	return BP
		// Validate that the actual declaration for this event has not been deleted 
		// either from a native base class or a BP multicast delegate. The Delegate could have been 
		// renamed/redirected, so also check for a remapped field if we need to
		&& (GetTargetDelegateProperty() || FMemberReference::FindRemappedField<FMulticastDelegateProperty>(DelegateOwnerClass, DelegatePropertyName));
}

bool UK2Node_BindingEase::HasDeprecatedReference() const
{
	if (CVarBPEnableDeprecatedWarningForComponentDelegateNodes.GetValueOnGameThread())
	{
		if (const FMulticastDelegateProperty* DelegateProperty = GetTargetDelegateProperty())
		{
			return DelegateProperty->HasAnyPropertyFlags(EPropertyFlags::CPF_Deprecated);
		}
	}
	return false;
}

FEdGraphNodeDeprecationResponse UK2Node_BindingEase::GetDeprecationResponse(EEdGraphNodeDeprecationType DeprecationType) const
{
	FEdGraphNodeDeprecationResponse Response = Super::GetDeprecationResponse(DeprecationType);
	if (DeprecationType == EEdGraphNodeDeprecationType::NodeHasDeprecatedReference)
	{
		const UFunction* Function = EventReference.ResolveMember<UFunction>(GetBlueprintClassFromNode());
		if (ensureMsgf(Function != nullptr, TEXT("This node should not be able to report having a deprecated reference if the event override cannot be resolved.")))
		{
			Response.MessageType = EEdGraphNodeDeprecationMessageType::Warning;
			const FText DetailedMessage = FText::FromString(Function->GetMetaData(FBlueprintMetadata::MD_DeprecationMessage));
			Response.MessageText = FBlueprintEditorUtils::GetDeprecatedMemberUsageNodeWarning(GetTargetDelegateDisplayName(), DetailedMessage);
		}
	}

	return Response;
}

bool UK2Node_BindingEase::IsUsedByAuthorityOnlyDelegate() const
{
	FMulticastDelegateProperty* TargetDelegateProp = GetTargetDelegateProperty();
	return (TargetDelegateProp && TargetDelegateProp->HasAnyPropertyFlags(CPF_BlueprintAuthorityOnly));
}

FMulticastDelegateProperty* UK2Node_BindingEase::GetTargetDelegateProperty() const
{
	return FindFProperty<FMulticastDelegateProperty>(DelegateOwnerClass, DelegatePropertyName);
}

FText UK2Node_BindingEase::GetTargetDelegateDisplayName() const
{
	FMulticastDelegateProperty* Prop = GetTargetDelegateProperty();
	return Prop ? Prop->GetDisplayNameText() : FText::FromName(DelegatePropertyName);
}

FText UK2Node_BindingEase::GetTooltipText() const
{
	FMulticastDelegateProperty* TargetDelegateProp = GetTargetDelegateProperty();
	if (TargetDelegateProp)
	{
		return TargetDelegateProp->GetToolTipText();
	}
	else
	{
		return FText::FromName(DelegatePropertyName);
	}
}

FString UK2Node_BindingEase::GetDocumentationLink() const
{
	if (DelegateOwnerClass)
	{
		return FString::Printf(TEXT("Shared/GraphNodes/Blueprint/%s%s"), DelegateOwnerClass->GetPrefixCPP(), *EventReference.GetMemberName().ToString());
	}

	return FString();
}

FString UK2Node_BindingEase::GetDocumentationExcerptName() const
{
	return DelegatePropertyName.ToString();
}

void UK2Node_BindingEase::ReconstructNode()
{
	// We need to fixup our event reference as it may have changed or been redirected
	FMulticastDelegateProperty* TargetDelegateProp = GetTargetDelegateProperty();

	// If we couldn't find the target delegate, then try to find it in the property remap table
	if (!TargetDelegateProp)
	{
		FMulticastDelegateProperty* NewProperty = FMemberReference::FindRemappedField<FMulticastDelegateProperty>(DelegateOwnerClass, DelegatePropertyName);
		if (NewProperty)
		{
			// Found a remapped property, update the node
			TargetDelegateProp = NewProperty;
			DelegatePropertyName = NewProperty->GetFName();
		}
	}

	if (TargetDelegateProp && TargetDelegateProp->SignatureFunction)
	{
		EventReference.SetFromField<UFunction>(TargetDelegateProp->SignatureFunction, false);
	}

	CachedNodeTitle.MarkDirty();

	Super::ReconstructNode();
}

void UK2Node_BindingEase::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	// Fix up legacy nodes that may not yet have a delegate pin
	if (Ar.IsLoading())
	{
		if (Ar.UEVer() < VER_UE4_K2NODE_EVENT_MEMBER_REFERENCE)
		{
			DelegateOwnerClass = EventSignatureClass_DEPRECATED;
		}

		// Recover from the period where DelegateOwnerClass was transient
		if (!DelegateOwnerClass && HasValidBlueprint())
		{
			// Search for a component property on the owning class, this should work in most cases
			UBlueprint* ParentBlueprint = GetBlueprint();
			UClass* ParentClass = ParentBlueprint ? ParentBlueprint->GeneratedClass : NULL;
			if (!ParentClass && ParentBlueprint)
			{
				// Try the skeleton class
				ParentClass = ParentBlueprint->SkeletonGeneratedClass;
			}

			#if 0 // todo
			FObjectProperty* ComponentProperty = ParentClass ? CastField<FObjectProperty>(ParentClass->FindPropertyByName(ComponentPropertyName)) : NULL;

			if (ParentClass && ComponentProperty)
			{
				UE_LOG(LogBlueprint, Warning, TEXT("Repaired invalid component bound event in node %s."), *GetPathName());
				DelegateOwnerClass = ComponentProperty->PropertyClass;
			}
			#endif
		}
	}
}

void UK2Node_BindingEase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CachedNodeTitle.Clear();
	CachedTooltip.Clear();

	InitializeComponentBoundEventParams(GetTargetDelegateProperty());
}


#undef LOCTEXT_NAMESPACE
