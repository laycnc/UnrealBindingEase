// Fill out your copyright notice in the Description page of Project Settings.


#include "BindingEaseLibrary.h"
#include "BindingEaseDelegateBinding.h"
#include "Engine/BlueprintGeneratedClass.h"

namespace BindingEase
{

	void Foreach(UObject* InBinding, UObject* InDelegateOwner, void (*InPred)(UObject*, UObject*, const FBlueprintBindingEaseDelegateBinding&))
	{
		UClass* SelfType = InBinding->GetClass();
		const UBlueprintGeneratedClass* BPGC = Cast<UBlueprintGeneratedClass>(SelfType);
		if (!IsValid(BPGC))
		{
			return;
		}
		for (const UDynamicBlueprintBinding* DynamicBindingObject : BPGC->DynamicBindingObjects)
		{
			const UBindingEaseDelegateBinding* Binding = Cast<UBindingEaseDelegateBinding>(DynamicBindingObject);
			if (Binding == nullptr)
			{
				continue;
			}

			for (const FBlueprintBindingEaseDelegateBinding& DelegateBinding : Binding->DelegateBindings)
			{
				if (InDelegateOwner->GetClass()->IsChildOf(DelegateBinding.DelegateOwnerClass))
				{
					InPred( InBinding, InDelegateOwner, DelegateBinding );
				}
			}
		}
	}
	
	void RegisterBinding(UObject* InBinding, UObject* InDelegateOwner, const FBlueprintBindingEaseDelegateBinding& DelegateBinding)
	{
		auto* DelegateProperty = FindFProperty<FMulticastDelegateProperty>(DelegateBinding.DelegateOwnerClass, DelegateBinding.DelegatePropertyName);

		FScriptDelegate ScriptDelegate;
		ScriptDelegate.BindUFunction(InBinding, DelegateBinding.FunctionNameToBind);

		DelegateProperty->AddDelegate(MoveTemp(ScriptDelegate), InDelegateOwner);

	}


	void UnregisterBinding(UObject* InBinding, UObject* InDelegateOwner, const FBlueprintBindingEaseDelegateBinding& DelegateBinding)
	{
		auto* DelegateProperty = FindFProperty<FMulticastDelegateProperty>(DelegateBinding.DelegateOwnerClass, DelegateBinding.DelegatePropertyName);

		FScriptDelegate ScriptDelegate;
		ScriptDelegate.BindUFunction(InBinding, DelegateBinding.FunctionNameToBind);

		DelegateProperty->RemoveDelegate(MoveTemp(ScriptDelegate), InDelegateOwner);
	}

}

void UBindingEaseLibrary::RegisterBinding(UObject* InBinding, UObject* InDelegateOwner)
{
	check(InBinding);
	if (InDelegateOwner)
	{
		BindingEase::Foreach(InBinding, InDelegateOwner, &BindingEase::RegisterBinding);
	}
}

void UBindingEaseLibrary::UnregisterBinding(UObject* InBinding, UObject* InDelegateOwner)
{
	check(InBinding);
	if (InDelegateOwner)
	{
		BindingEase::Foreach(InBinding, InDelegateOwner, &BindingEase::UnregisterBinding);
	}
}