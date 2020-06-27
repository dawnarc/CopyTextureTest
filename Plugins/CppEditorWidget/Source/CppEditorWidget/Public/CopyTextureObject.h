// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CopyTextureObject.generated.h"

class UTexture;

/**
 * 
 */
UCLASS(BlueprintType)
class CPPEDITORWIDGET_API UCopyTextureObject
	: public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UTexture* SourceTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UTexture* DestinationTexture;
};
