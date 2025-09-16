// UIExtension.Build.cs

using UnrealBuildTool;

public class UIExtension : ModuleRules
{
	public UIExtension(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(
			new string[] {
				// Unreal Engine
				"Core", "CoreUObject", "Engine",

				// UI
				"SlateCore", "Slate", "UMG",
				
				// CommonGame
				"CommonUI", "CommonGame", "GameplayTags",
			}
		);
	}
}