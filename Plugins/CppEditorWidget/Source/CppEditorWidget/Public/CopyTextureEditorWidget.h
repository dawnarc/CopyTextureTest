// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CopyTextureEditorWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPPEDITORWIDGET_API UCopyTextureEditorWidget : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UTexture* SrcTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UTexture* DestTexture;
};
