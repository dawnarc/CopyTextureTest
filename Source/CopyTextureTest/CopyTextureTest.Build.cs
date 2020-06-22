// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CopyTextureTest : ModuleRules
{
	public CopyTextureTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        if (Target.bBuildEditor == true)
        {
            PublicDependencyModuleNames.AddRange(
                new string[] {
                "UnrealEd",
                "AssetTools",
            });
        }
    }
}
