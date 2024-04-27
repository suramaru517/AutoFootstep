// Copyright 2023-2024 Metaseven. All Rights Reserved.

using UnrealBuildTool;

public class AutoFootstepEditor : ModuleRules
{
	public AutoFootstepEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"AnimationBlueprintLibrary",
				"AnimationModifiers",
				"AutoFootstep",
			}
		);
	}
}
