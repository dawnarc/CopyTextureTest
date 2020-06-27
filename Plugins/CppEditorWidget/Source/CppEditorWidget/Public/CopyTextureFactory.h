// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CopyTextureFactory.generated.h"

/**
 * 
 */
UCLASS()
class CPPEDITORWIDGET_API UCopyTextureFactory
	: public UFactory
{
	GENERATED_BODY()
	
public:
	UCopyTextureFactory();

	//~ Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface	
};
