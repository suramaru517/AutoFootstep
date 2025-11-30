// Copyright Metaseven. All Rights Reserved.

using UnrealBuildTool;

public class AutoFootstepEditor : ModuleRules
{
	public AutoFootstepEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"CoreUObject",
			"Engine",
			"AnimationBlueprintLibrary",
			"AnimationModifiers",
			"AutoFootstep",
		});
	}
}
