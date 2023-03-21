// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/DynamicBlueprintBinding.h"
#include "SimplePropertyDelegateBinding.generated.h"

class UInputComponent;

USTRUCT()
struct FBlueprintSimplePropertyDelegateBinding
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TObjectPtr<UClass> DelegateOwnerClass;


	/** Name of property on the component that we want to assign to. */
	UPROPERTY()
	FName DelegatePropertyName = NAME_None;

	/** Name of function that we want to bind to the delegate. */
	UPROPERTY()
	FName FunctionNameToBind = NAME_None;
};

UCLASS()
class BINDINGEASE_API USimplePropertyDelegateBinding
	: public UDynamicBlueprintBinding
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY()
	TArray<FBlueprintSimplePropertyDelegateBinding> DelegateBindings;


};