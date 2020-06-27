using UnrealBuildTool;

public class CppEditorWidget : ModuleRules
{
	public CppEditorWidget(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
            "Slate",
            "SlateCore",
            "UnrealEd",
            "Blutility",
            "UMG",
            "UMGEditor",
			"RHI",
			"RenderCore"
		});

        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.AddRange(
                new string[] {
                    "AssetTools"
                });
        }

		PrivateDependencyModuleNames.AddRange(new string[] { });

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
