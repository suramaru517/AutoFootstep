// Copyright Metaseven. All Rights Reserved.

using UnrealBuildTool;

public class AutoFootstep : ModuleRules
{
	public AutoFootstep(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"Niagara",
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"CoreUObject",
			"Engine",
			"AIModule",
			"PhysicsCore",
		});
	}
}
