// ShooterX.Build.cs

using UnrealBuildTool;

public class ShooterX : ModuleRules
{
	public ShooterX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			// Initial Module
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			
			// ModularGameplay Public Dependency Modules
			"GameplayTags",

			// GameStateComponent
			"ModularGameplay",
			
			// GameFeaturesSubsystemSettings
			"GameFeatures",
			
			// CommonUser
			"CommonUser",
			
			// Game Features
			"ModularGameplayActor",
			
			// GameplayAbilitySystem
			"GameplayTasks",
			"GameplayAbilities", // GameplayTasks 모듈에 Dependency 있음.

			"CommonGame",
			
			// CommonUI
			"CommonUI",

			// UMG
			"UMG",
			
			"UIExtension",
			
			// Slate
			"Slate",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[] { "ShooterX", });
	}
}
