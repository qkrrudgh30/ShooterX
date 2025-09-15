// CommonGame.Build.cs

using UnrealBuildTool;

public class CommonGame : ModuleRules
{
	public CommonGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			// Initial Modules
			"Core", "CoreUObject", "Engine", "InputCore",

			// UI
			"Slate", "SlateCore", "UMG", "CommonUI", "CommonUser",

			// GameplayFramework
			"GameplayTags", "ModularGameplayActor"

		});
	}
}
