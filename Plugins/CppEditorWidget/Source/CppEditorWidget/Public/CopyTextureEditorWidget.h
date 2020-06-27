#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"

#include "CopyTextureEditorWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CPPEDITORWIDGET_API UCopyTextureEditorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	
	UCopyTextureEditorWidget();

	void NativeConstruct() override;

protected:

	UFUNCTION()
		void OnBtnCopyTextureClick();

	void TestBytesToTexture();

	void CopyTextureTest();

	UPROPERTY()
	class UDetailsView* DetTexture2Copy;

	TRefCountPtr<FRHITexture2D> OutputTarget;
};
