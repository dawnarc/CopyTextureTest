#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"

#include "MyEditorWidgetClass.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CPPEDITORWIDGET_API UMyEditorWidgetClass : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	
	UMyEditorWidgetClass();

	void NativeConstruct() override;

protected:

	UFUNCTION()
		void OnBtnCopyTextureClick();

	void TestBytesToTexture();

	void TestFun001();

	UPROPERTY()
	class UDetailsView* DetTexture2Copy;

	TRefCountPtr<FRHITexture2D> OutputTarget;
};
