// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BindingEaseLibrary.generated.h"

/**
 *
 */
UCLASS()
class UBindingEaseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "BindingEase", meta = (HidePin = "InBinding", DefaultToSelf = "InBinding"))
	static BINDINGEASE_API void RegisterBinding(UObject* InBinding, UObject* InDelegateOwner);

	UFUNCTION(BlueprintCallable, Category = "BindingEase", meta = (HidePin = "InBinding", DefaultToSelf = "InBinding"))
	static BINDINGEASE_API void UnregisterBinding(UObject* InBinding, UObject* InDelegateOwner);


};
