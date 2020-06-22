// Copyright Epic Games, Inc. All Rights Reserved.

#include "CopyTextureTest.h"

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"
#include "Modules/ModuleInterface.h"

#include "AssetTypeActions_CopyTexture.h"

class IGeoDeviceServicesProxyManager;

#define LOCTEXT_NAMESPACE "PrimaryModule"

class FPrimaryModule
	: public IModuleInterface
{
	// IModuleInterface
	virtual void StartupModule() override
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		TSharedPtr<FAssetTypeActions_CopyTexture> AssetTypeActions_CopyTexture = MakeShared<FAssetTypeActions_CopyTexture>();
		AssetTools.RegisterAssetTypeActions(AssetTypeActions_CopyTexture.ToSharedRef());
		CreatedAssetTypeActions.Add(AssetTypeActions_CopyTexture);
	}

	virtual void ShutdownModule() override
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

private:
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE(FPrimaryModule, CopyTextureTest, "CopyTextureTest");
