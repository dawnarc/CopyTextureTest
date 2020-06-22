// Fill out your copyright notice in the Description page of Project Settings.


#include "CopyTextureFactory.h"
#include "CopyTextureObject.h"

UCopyTextureFactory::UCopyTextureFactory()
{
	SupportedClass = UCopyTextureObject::StaticClass();

	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCopyTextureFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UCopyTextureObject>(InParent, InClass, InName, Flags);
}
