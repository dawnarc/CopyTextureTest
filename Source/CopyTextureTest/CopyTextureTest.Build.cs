// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CopyTextureTest : ModuleRules
{
	public CopyTextureTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", 
            "Slate",
            "SlateCore",
            "UnrealEd",
            "Blutility",
            "UMG",
            "UMGEditor"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "CppEditorWidget" });
    }
}
