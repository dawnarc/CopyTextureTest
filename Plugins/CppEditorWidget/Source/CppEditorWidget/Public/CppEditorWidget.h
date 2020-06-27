#pragma once

#include "CoreMinimal.h"
#include "IAssetTypeActions.h"
#include "Modules/ModuleManager.h"

class FCppEditorWidgetModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;
};
