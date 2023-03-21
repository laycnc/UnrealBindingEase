// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Templates/SharedPointer.h"
#include "UObject/WeakObjectPtr.h"

class IPropertyHandle;

/** 
 * A base class for details customization of UObjects in nDisplay. Contains support for common custom metadata specifiers.
 */
class FK2Node_BindingEaseDetailCustomization
	: public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	// IDetailCustomization interface
		/** Called when details should be customized */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	// End IDetailCustomization interface

private:

	void UpdateDelagateSource();

	void HandleComboBoxChanged(FName InItem, ESelectInfo::Type InSeletionInfo);
	TSharedRef<SWidget> HandleGenerateWidget(FName InItem);
	FText HandleComboBoxValueAsText() const;

private:
	TSharedPtr<IPropertyHandle> DelegateOwnerClassProperty;
	TSharedPtr<IPropertyHandle> DelegatePropertyNameProperty;
	TArray<FName> DelegateNames;
	TArray<FText> DelegateTooltips;

};