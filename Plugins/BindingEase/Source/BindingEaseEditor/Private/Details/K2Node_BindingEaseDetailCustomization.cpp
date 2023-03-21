// Copyright Epic Games, Inc. All Rights Reserved.

#include "Details/K2Node_BindingEaseDetailCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "IDocumentation.h"
#include "Widgets/SToolTip.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"
#include "K2Node_BindingEase.h"

TSharedRef<IDetailCustomization> FK2Node_BindingEaseDetailCustomization::MakeInstance()
{
	return MakeShared<FK2Node_BindingEaseDetailCustomization>();
}

// IDetailCustomization interface


void FK2Node_BindingEaseDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& InLayoutBuilder)
{
	IDetailCategoryBuilder& BindingEaseCategory = InLayoutBuilder.EditCategory("BindingEase");

	DelegateOwnerClassProperty = InLayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UK2Node_BindingEase, DelegateOwnerClass));
	DelegatePropertyNameProperty = InLayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UK2Node_BindingEase, DelegatePropertyName));

	BindingEaseCategory.AddProperty(DelegateOwnerClassProperty);
	IDetailPropertyRow& DelegatePropertyNamePropertyRow = BindingEaseCategory.AddProperty(DelegatePropertyNameProperty);

	const auto EditCondition = TAttribute<bool>::CreateLambda([this]() -> bool
		{
			UObject* Result = nullptr;
			if (DelegateOwnerClassProperty->GetValue(Result) == FPropertyAccess::Success)
			{
				// 
				return Result != nullptr;
			}
			return false;
		});

	DelegatePropertyNamePropertyRow.EditCondition(EditCondition, FOnBooleanValueChanged());

	UpdateDelagateSource();

	DelegatePropertyNamePropertyRow
		.CustomWidget()
		.NameContent()
		[
			DelegatePropertyNameProperty->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SComboBox<FName>)
			.OptionsSource(&DelegateNames)
			.OnSelectionChanged(this, &FK2Node_BindingEaseDetailCustomization::HandleComboBoxChanged)
			.OnGenerateWidget(this, &FK2Node_BindingEaseDetailCustomization::HandleGenerateWidget)
			.Content()
			[
				SNew(STextBlock)
				.Text(this, &FK2Node_BindingEaseDetailCustomization::HandleComboBoxValueAsText)
			]
		];

}


void FK2Node_BindingEaseDetailCustomization::UpdateDelagateSource()
{
	DelegateNames.Empty();
	DelegateTooltips.Empty();

	UObject* Result = nullptr;
	if (DelegateOwnerClassProperty->GetValue(Result) != FPropertyAccess::Success)
	{
		return;
	}

	UClass* BindingClass = Cast<UClass>(Result);
	for (auto* DelegateProperty : TFieldRange<FMulticastDelegateProperty>(BindingClass))
	{
		DelegateNames.Add(DelegateProperty->GetFName());
		DelegateTooltips.Add(DelegateProperty->GetToolTipText());
	}
}

void FK2Node_BindingEaseDetailCustomization::HandleComboBoxChanged(FName InItem, ESelectInfo::Type InSeletionInfo)
{
	if (DelegatePropertyNameProperty.IsValid())
	{
		DelegatePropertyNameProperty->SetValue(InItem);
	}
}

TSharedRef<SWidget> FK2Node_BindingEaseDetailCustomization::HandleGenerateWidget(FName InItem)
{
	int32 Index = DelegateNames.Find(InItem);
	FText Tooltip = DelegateTooltips.IsValidIndex(Index) ? DelegateTooltips[Index] : FText::GetEmpty();

	return SNew(STextBlock)
		.Text(FText::FromName(InItem))
		.ToolTip(IDocumentation::Get()->CreateToolTip(Tooltip, nullptr, TEXT(""), TEXT("")))
		;
}

FText FK2Node_BindingEaseDetailCustomization::HandleComboBoxValueAsText() const
{
	if (DelegatePropertyNameProperty.IsValid())
	{
		FName Value = NAME_None;
		if (DelegatePropertyNameProperty->GetValue(Value) == FPropertyAccess::Success)
		{
			return FText::FromName(Value);
		}
	}
	return FText::GetEmpty();
}

// End IDetailCustomization interface
