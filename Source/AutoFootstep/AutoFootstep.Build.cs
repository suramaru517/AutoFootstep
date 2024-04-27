// Copyright 2023-2024 Metaseven. All Rights Reserved.

using UnrealBuildTool;

public class AutoFootstep : ModuleRules
{
	public AutoFootstep(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Niagara",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"AIModule",
				"PhysicsCore",
			}
		);
	}
}
