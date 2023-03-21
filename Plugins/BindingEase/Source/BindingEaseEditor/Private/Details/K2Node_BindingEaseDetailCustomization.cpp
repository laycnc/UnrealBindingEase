// Copyright Epic Games, Inc. All Rights Reserved.

#include "Details/K2Node_BindingEaseDetailCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "K2Node_BindingEase.h"

TSharedRef<IDetailCustomization> FK2Node_BindingEaseDetailCustomization::MakeInstance()
{
	return MakeShared<FK2Node_BindingEaseDetailCustomization>();
}

// IDetailCustomization interface


void FK2Node_BindingEaseDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& InLayoutBuilder)
{
	IDetailCategoryBuilder& BindingEaseCategory = InLayoutBuilder.EditCategory("BindingEase");

	auto DelegateOwnerClassProperty = InLayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UK2Node_BindingEase, DelegateOwnerClass));
	auto DelegatePropertyNameProperty = InLayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UK2Node_BindingEase, DelegatePropertyName));

	BindingEaseCategory.AddProperty(DelegateOwnerClassProperty);
	IDetailPropertyRow& DelegatePropertyNamePropertyRow = BindingEaseCategory.AddProperty(DelegatePropertyNameProperty);

	const auto EditCondition = TAttribute<bool>::CreateLambda([DelegateOwnerClassProperty]() -> bool
		{
			UObject* Result = nullptr;
			if( DelegateOwnerClassProperty->GetValue(Result) == FPropertyAccess::Success)
			{
				// 
				return Result != nullptr;
			}
			return false;
		});

	DelegatePropertyNamePropertyRow.EditCondition(EditCondition, FOnBooleanValueChanged());
}

// End IDetailCustomization interface
