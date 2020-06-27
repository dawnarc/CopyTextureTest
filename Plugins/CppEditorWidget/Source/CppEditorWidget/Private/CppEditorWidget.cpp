#include "CppEditorWidget.h"

#include "AssetTypeActions_CopyTexture.h"

#define LOCTEXT_NAMESPACE "FCppEditorWidgetModule"

void FCppEditorWidgetModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	TSharedPtr<FAssetTypeActions_CopyTexture> AssetTypeActions_CopyTexture = MakeShared<FAssetTypeActions_CopyTexture>();
	AssetTools.RegisterAssetTypeActions(AssetTypeActions_CopyTexture.ToSharedRef());
	CreatedAssetTypeActions.Add(AssetTypeActions_CopyTexture);
}

void FCppEditorWidgetModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (auto CreatedAssetTypeAction : CreatedAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeAction.ToSharedRef());
		}
	}
	CreatedAssetTypeActions.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCppEditorWidgetModule, CppEditorWidget)
