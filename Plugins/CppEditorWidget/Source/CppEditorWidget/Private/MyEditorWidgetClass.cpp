#include "MyEditorWidgetClass.h"

#include "Async/Async.h"
#include "Engine/Texture2D.h"

#include "Components/Button.h"
#include "Components/DetailsView.h"
#include "CopyTextureEditorWidget.h"
#include "CopyTextureObject.h"
#include "Field/FieldSystemNoiseAlgo.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"

#define DEBUG_MSG(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT(x), __VA_ARGS__));}

//Render thread wrapper struct
struct FUpdateTextureData
{
	UTexture2D* Texture2D;
	FUpdateTextureRegion2D Region;
	uint32 Pitch;
	const TArray<uint8>* BufferArray;
	TSharedPtr<IImageWrapper> Wrapper;	//to keep the uncompressed data alive
};

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
	//TestFun001();
	TestBytesToTexture();
}

void UMyEditorWidgetClass::TestBytesToTexture()
{
	//testing image path
	FString ImagePath = { "E:\\Protoss\\Desktop\\CopyTextureTest\\Sven_icon.png" };

	//image raw data
	TArray<uint8> ImageRawData;
	FFileHelper::LoadFileToArray(ImageRawData, *ImagePath);
	
	//Convert the UTexture2D back to an image
	UTexture2D* Texture = nullptr;

	static IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat DetectedFormat = ImageWrapperModule.DetectImageFormat(ImageRawData.GetData(), ImageRawData.Num());

	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(DetectedFormat);

	//Set the compressed bytes - we need this information on game thread to be able to determine texture size, otherwise we'll need a complete async callback
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ImageRawData.GetData(), ImageRawData.Num()))
	{
		//Create image given sizes
		Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
		Texture->UpdateResource();

		//Uncompress on a background thread pool
		Async(EAsyncExecution::ThreadPool, [ImageWrapper, Texture] {
			TArray<uint8> UncompressedBGRA;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{

				FUpdateTextureData* UpdateData = new FUpdateTextureData;
				UpdateData->Texture2D = Texture;
				UpdateData->Region = FUpdateTextureRegion2D(0, 0, 0, 0, Texture->GetSizeX(), Texture->GetSizeY());
				UpdateData->BufferArray = &UncompressedBGRA;
				UpdateData->Pitch = Texture->GetSizeX() * 4;
				UpdateData->Wrapper = ImageWrapper;

				ENQUEUE_RENDER_COMMAND(UpdateTextureDataCommand)([UpdateData](FRHICommandListImmediate& RHICmdList) {
					RHIUpdateTexture2D(
						((FTexture2DResource*)UpdateData->Texture2D->Resource)->GetTexture2DRHI(),
						0,
						UpdateData->Region,
						UpdateData->Pitch,
						UpdateData->BufferArray->GetData()
					);
					delete UpdateData; //now that we've updated the texture data, we can finally release any data we're holding on to
					});//End Enqueue
			}
			}
			);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid image format cannot decode %d"), (int32)DetectedFormat);
	}

	if(Texture)
	{
		if (DetTexture2Copy)
		{
			if (UCopyTextureObject* CopyWidget = Cast<UCopyTextureObject>(DetTexture2Copy->GetObject()))
			{
				CopyWidget->DestinationTexture = Texture;
			}
		}
	}
}

void UMyEditorWidgetClass::TestFun001()
{
	if (DetTexture2Copy)
	{
		if (UCopyTextureObject* CopyWidget = Cast<UCopyTextureObject>(DetTexture2Copy->GetObject()))
		{
			/*if (UTexture2D* SrcTex2D = Cast<UTexture2D>(CopyWidget->SourceTexture))
			{
				FString NameSrc = SrcTex2D->GetPathName();
				DEBUG_MSG("SRC+++: %s", *NameSrc);
			}

			if (UTexture2D* DestTex2D = Cast<UTexture2D>(CopyWidget->DestinationTexture))
			{
				FString NameDest = DestTex2D->GetPathName();
				DEBUG_MSG("DEST+++: %s", *NameDest);
			}*/

			UTexture2D* SrcTex2D = Cast<UTexture2D>(CopyWidget->SourceTexture);
			UTexture2D* DestTex2D = Cast<UTexture2D>(CopyWidget->DestinationTexture);

			if (SrcTex2D && DestTex2D)
			{
				/*const uint32 OutputCreateFlags = TexCreate_Dynamic | TexCreate_SRGB;

				FRHIResourceCreateInfo CreateInfo = {
					FClearValueBinding(FLinearColor::Transparent)
				};

				TRefCountPtr<FRHITexture2D> DummyTexture2DRHI;

				RHICreateTargetableShaderResource2D(
					DestTex2D->GetSizeX(),
					DestTex2D->GetSizeY(),
					DestTex2D->GetPixelFormat(),
					1,
					OutputCreateFlags,
					TexCreate_RenderTargetable,
					false,
					CreateInfo,
					OutputTarget,
					DummyTexture2DRHI
				);*/

				FTexture2DRHIRef RenderTargetTextureRHI;
				FTexture2DRHIRef ShaderResourceUnused;
				FRHIResourceCreateInfo CreateInfo;

				RHICreateTargetableShaderResource2D(
					DestTex2D->GetSizeX(),
					DestTex2D->GetSizeY(),
					PF_B8G8R8A8,
					1,
					/*TexCreateFlags=*/0,
					TexCreate_RenderTargetable,
					/*bNeedsTwoCopies=*/false,
					CreateInfo,
					OutputTarget,
					ShaderResourceUnused
				);

				//if (RenderTargetTextureRHI != OutputTarget)
				{
					//RHIUpdateTextureReference(SrcTex2D->TextureReference.TextureReferenceRHI, OutputTarget);
				}

				/*FUpdateTextureData* UpdateData = new FUpdateTextureData;
				UpdateData->Texture2D = Holder->Texture;
				UpdateData->Region = FUpdateTextureRegion2D(0, 0, 0, 0, Size.X, Size.Y);
				UpdateData->BufferArray = UncompressedBGRA;
				UpdateData->Pitch = Size.X * 4;
				UpdateData->Wrapper = ImageWrapper;*/

				//FUpdateTextureData* UpdateData = new FUpdateTextureData;
				//UpdateData->Texture2D = DestTex2D;
				//UpdateData->Region = FUpdateTextureRegion2D(0, 0, 0, 0, DestTex2D->GetSizeX(), DestTex2D->GetSizeY());
				//UpdateData->BufferArray = UncompressedBGRA;
				//UpdateData->Pitch = Texture->GetSizeX() * 4;

				////This command sends it to the render thread
				//ENQUEUE_RENDER_COMMAND(
				//	FUpdateTextureData,
				//	FUpdateTextureData*, UpdateData, UpdateData,
				//	{
				//		RHIUpdateTexture2D(
				//			((FTexture2DResource*)UpdateData->Texture2D->Resource)->GetTexture2DRHI(),
				//			0,
				//			UpdateData->Region,
				//			UpdateData->Pitch,
				//			UpdateData->BufferArray->GetData()
				//		);
				//		delete UpdateData; //now that we've updated the texture data, we can finally release any data we're holding on to
				//	});//End Enqueue

				//// copy sample data to output render target
				//FUpdateTextureRegion2D Region(0, 0, 0, 0, DestTex2D->GetSizeX(), DestTex2D->GetSizeY());
				//RHIUpdateTexture2D(OutputTarget.GetReference(), 0, Region, SrcTex2D->Source.GetSizeX(), (uint8*)SrcTex2D->ResourceMem->GetMipData(0));
			}
		}
	}
}