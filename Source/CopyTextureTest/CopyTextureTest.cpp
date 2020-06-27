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
	}

	virtual void ShutdownModule() override
	{
	}
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE(FPrimaryModule, CopyTextureTest, "CopyTextureTest");
