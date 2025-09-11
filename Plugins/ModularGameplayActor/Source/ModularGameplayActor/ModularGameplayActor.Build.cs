// ModularGameplayActor.Build.cs

using UnrealBuildTool;
using System.IO;

public class ModularGameplayActor : ModuleRules
{
	public ModularGameplayActor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				// Initial
				"Core", "CoreUObject", "Engine", "ModularGameplay",
			}
		);
	}
}