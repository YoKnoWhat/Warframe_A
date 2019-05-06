// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Warframe_A : ModuleRules
{
	public Warframe_A(ReadOnlyTargetRules Target) : base(Target)
	{
		bUseRTTI = true;

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Niagara" });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "GameplayTasks", "Json", "UMGEditor", "AIModule", "NavigationSystem" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
