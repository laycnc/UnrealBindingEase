// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class BindingEaseNodes : ModuleRules
{
	public BindingEaseNodes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
			{ 
				"Core",
				"CoreUObject",
				"Engine",
				"BindingEase"
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// todo
				"InputCore",
				"SlateCore",
				"UnrealEd",
				"GraphEditor",
				"KismetCompiler",
				"BlueprintGraph",
			});
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				
			});
	}
}
