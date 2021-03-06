#include "CopyTextureEditorWidget.h"

#include "Async/Async.h"
#include "Engine/Texture2D.h"

#include "Components/Button.h"
#include "Components/DetailsView.h"
#include "CopyTextureObject.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageWriteQueue/Public/ImagePixelData.h"
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

UCopyTextureEditorWidget::UCopyTextureEditorWidget()
{
}

void UCopyTextureEditorWidget::NativeConstruct()
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

void UCopyTextureEditorWidget::OnBtnCopyTextureClick()
{
	CopyTextureTest();
	//TestBytesToTexture();
}

void UCopyTextureEditorWidget::TestBytesToTexture()
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

void UCopyTextureEditorWidget::CopyTextureTest()
{
	if (DetTexture2Copy)
	{
		if (UCopyTextureObject* CopyWidget = Cast<UCopyTextureObject>(DetTexture2Copy->GetObject()))
		{
			if (UTexture2D* SrcTex2D = Cast<UTexture2D>(CopyWidget->SourceTexture))
			{
				auto OnPixelsReady = [CopyWidget](TUniquePtr<FImagePixelData>&& PixelData)
				{
					if (PixelData.IsValid())
					{
						const void* PixelRawData = nullptr;
						int64 PixelRawDataSize = -1;
						PixelData->GetRawData(PixelRawData, PixelRawDataSize);
						
						if(PixelRawData && PixelRawDataSize > 0)
						{
							//UTexture2D write to
							UTexture2D* Texture = nullptr;

							//Create image given sizes
							if (CopyWidget->SourceTexture)
							{
								Texture = UTexture2D::CreateTransient(CopyWidget->SourceTexture->GetSurfaceWidth(), CopyWidget->SourceTexture->GetSurfaceHeight(), PF_B8G8R8A8);
								Texture->UpdateResource();
							}

							//Uncompress on a background thread pool
							Async(EAsyncExecution::ThreadPool, [PixelRawData, PixelRawDataSize, Texture] {
								TArray<uint8> UncompressedBGRA;
								UncompressedBGRA.Append((uint8*)PixelRawData, PixelRawDataSize);

								FUpdateTextureData* UpdateData = new FUpdateTextureData;
								UpdateData->Texture2D = Texture;
								UpdateData->Region = FUpdateTextureRegion2D(0, 0, 0, 0, Texture->GetSizeX(), Texture->GetSizeY());
								UpdateData->BufferArray = &UncompressedBGRA;
								UpdateData->Pitch = Texture->GetSizeX() * 4;

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
							);

							if (Texture)
							{
								CopyWidget->DestinationTexture = Texture;
							}
						}
					}
				};

				FTextureResource* TextureResource = SrcTex2D->Resource;
				ENQUEUE_RENDER_COMMAND(ResolvePixelData)(
					[TextureResource, OnPixelsReady](FRHICommandListImmediate& RHICmdList) {
						FTexture2DRHIRef Texture2D = TextureResource->TextureRHI ? TextureResource->TextureRHI->GetTexture2D() : nullptr;
						if (!Texture2D)
						{
							OnPixelsReady(nullptr);
							return;
						}

						FIntRect SourceRect(0, 0, Texture2D->GetSizeX(), Texture2D->GetSizeY());
						switch (Texture2D->GetFormat())
						{
						case PF_FloatRGBA:
						{
							TArray<FFloat16Color> RawPixels;
							RawPixels.SetNum(SourceRect.Width() * SourceRect.Height());
							RHICmdList.ReadSurfaceFloatData(Texture2D, SourceRect, RawPixels, (ECubeFace)0, 0, 0);

							TUniquePtr<TImagePixelData<FFloat16Color>> PixelData = MakeUnique<TImagePixelData<FFloat16Color>>(SourceRect.Size(), TArray64<FFloat16Color>(MoveTemp(RawPixels)));

							if (PixelData->IsDataWellFormed())
							{
								OnPixelsReady(MoveTemp(PixelData));
								return;
							}

							break;
						}
						case PF_A32B32G32R32F:
						{
							FReadSurfaceDataFlags ReadDataFlags(RCM_MinMax);
							ReadDataFlags.SetLinearToGamma(false);


							TArray<FLinearColor> RawPixels;
							RawPixels.SetNum(SourceRect.Width() * SourceRect.Height());
							RHICmdList.ReadSurfaceData(Texture2D, SourceRect, RawPixels, ReadDataFlags);
							TUniquePtr<TImagePixelData<FLinearColor>> PixelData = MakeUnique<TImagePixelData<FLinearColor>>(SourceRect.Size(), TArray64<FLinearColor>(MoveTemp(RawPixels)));
							if (PixelData->IsDataWellFormed())
							{
								OnPixelsReady(MoveTemp(PixelData));
								return;
							}

							break;
						}
						case PF_R8G8B8A8:
						case PF_B8G8R8A8:
						{
							FReadSurfaceDataFlags ReadDataFlags;
							ReadDataFlags.SetLinearToGamma(false);

							TArray<FColor> RawPixels;
							RawPixels.SetNum(SourceRect.Width() * SourceRect.Height());
							RHICmdList.ReadSurfaceData(Texture2D, SourceRect, RawPixels, ReadDataFlags);

							TUniquePtr<TImagePixelData<FColor>> PixelData = MakeUnique<TImagePixelData<FColor>>(SourceRect.Size(), TArray64<FColor>(MoveTemp(RawPixels)));
							if (PixelData->IsDataWellFormed())
							{
								OnPixelsReady(MoveTemp(PixelData));
								return;
							}

							break;
						}
						default:
							break;
						}

						OnPixelsReady(nullptr);
					}
				);
			}
		}
	}
}