// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_CopyTexture.h"
#include "Toolkits/SimpleAssetEditor.h"

void FAssetTypeActions_CopyTexture::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);
}
