#include "MyEditorWidgetClass.h"
#include "Engine/Texture2D.h"

#include "Components/Button.h"
#include "Components/DetailsView.h"
#include "CopyTextureEditorWidget.h"
#include "CopyTextureObject.h"

#define DEBUG_MSG(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT(x), __VA_ARGS__));}

UMyEditorWidgetClass::UMyEditorWidgetClass()
{
}

void UMyEditorWidgetClass::NativeConstruct()
{
	Super::NativeConstruct();

    if (UButton* btn = Cast<UButton>(GetWidgetFromName("BtnCopyTexture")))
    {
        FScriptDelegate Del;
        Del.BindUFunction(this, "OnBtnCopyTextureClick");
        btn->OnClicked.Add(Del);
    }

	DetTexture2Copy = Cast<UDetailsView>(GetWidgetFromName("DetSrcAndDestTex"));
}

void UMyEditorWidgetClass::OnBtnCopyTextureClick()
{
	if(DetTexture2Copy)
	{
        if(UCopyTextureObject* CopyWidget = Cast<UCopyTextureObject>(DetTexture2Copy->GetObject()))
        {
			if (UTexture2D* SrcTex2D = Cast<UTexture2D>(CopyWidget->SourceTexture))
			{
				FString NameSrc = SrcTex2D->GetPathName();
				DEBUG_MSG("SRC+++: %s", *NameSrc);
			}

			if (UTexture2D* DestTex2D = Cast<UTexture2D>(CopyWidget->DestinationTexture))
			{
				FString NameDest = DestTex2D->GetPathName();
				DEBUG_MSG("DEST+++: %s", *NameDest);
			}
        }
	}
}