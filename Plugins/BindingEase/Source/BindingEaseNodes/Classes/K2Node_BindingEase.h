// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraph.h"
#include "Textures/SlateIcon.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "BlueprintNodeSignature.h"
#include "K2Node_Event.h"

#include "K2Node_BindingEase.generated.h"


class FBlueprintActionDatabaseRegistrar;
class FKismetCompilerContext;

UCLASS(MinimalAPI)
class UK2Node_BindingEase
	: public UK2Node_Event
{
	GENERATED_UCLASS_BODY()

public:
	// The key that is bound
	UPROPERTY(EditAnywhere, Category="BindingEase")
	TObjectPtr<UClass> DelegateOwnerClass;

	// Should the binding execute even when the game is paused
	UPROPERTY(EditAnywhere, Category="BindingEase")
	FName DelegatePropertyName;


	//~ Begin UObject Interface
	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	virtual void Serialize(FArchive& Ar) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface
	virtual void ReconstructNode() override;
	virtual bool CanPasteHere(const UEdGraph* TargetGraph) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FString GetDocumentationLink() const override;
	virtual FString GetDocumentationExcerptName() const override;
	virtual bool HasDeprecatedReference() const override;
	virtual FEdGraphNodeDeprecationResponse GetDeprecationResponse(EEdGraphNodeDeprecationType DeprecationType) const override;
	//~ End UEdGraphNode Interface

	//~ Begin UK2Node Interface
	virtual bool ShouldShowNodeProperties() const override { return true; }
	virtual bool NodeCausesStructuralBlueprintChange() const override { return true; }
	virtual UClass* GetDynamicBindingClass() const override;
	virtual void RegisterDynamicBinding(UDynamicBlueprintBinding* BindingObject) const override;
	virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	//~ End UK2Node Interface

	virtual bool IsUsedByAuthorityOnlyDelegate() const override;

	/** Return the delegate property that this event is bound to */
	FMulticastDelegateProperty* GetTargetDelegateProperty() const;

	/** Gets the proper display name for the property */
	FText GetTargetDelegateDisplayName() const;

	void InitializeComponentBoundEventParams(const FMulticastDelegateProperty* InDelegateProperty);

private:

	/** Returns true if there is an FObjectProperty on this blueprint with a name that matches ComponentPropertyName */
	bool IsDelegateValid() const;

	/** Constructing FText strings can be costly, so we cache the node's title */
	FNodeTextCache CachedTooltip;
	FNodeTextCache CachedNodeTitle;
};
