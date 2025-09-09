// CommonUser.Build.cs

using UnrealBuildTool;

public class CommonUser : ModuleRules
{
	public CommonUser(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			// Initial Modules
			"Core", "CoreUObject", "Engine"
		});

	}
}
