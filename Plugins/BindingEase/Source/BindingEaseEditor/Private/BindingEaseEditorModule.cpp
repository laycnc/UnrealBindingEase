// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "K2Node_BindingEase.h"
#include "Details/K2Node_BindingEaseDetailCustomization.h"

class FBindingEaseEditorModule
	: public IModuleInterface
{
		
public:
	void StartupModule() override final
	{
		// Register the details customizations
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.RegisterCustomClassLayout(UK2Node_BindingEase::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FK2Node_BindingEaseDetailCustomization::MakeInstance));
		}
	}

	void ShutdownModule() override final
	{
		// Unregister the details customizations
		if(FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
		{
			PropertyModule->UnregisterCustomClassLayout(UK2Node_BindingEase::StaticClass()->GetFName());
		}
	}

};
	
IMPLEMENT_MODULE(FBindingEaseEditorModule, BindingEaseEditor);