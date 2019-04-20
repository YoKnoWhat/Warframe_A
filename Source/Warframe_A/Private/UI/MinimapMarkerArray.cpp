
#include "UI/MinimapMarkerArray.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "UI/Minimap.h"

#include "Runtime/Engine/Classes/Engine/Texture2D.h"


FMinimapMarkerArray::FMinimapMarkerArray()
{}

void FMinimapMarkerArray::Init(UMinimap* InOwner, const FName& InTextureName, const FVector2D& InImageSize)
{
	Owner = InOwner;

	Brush.SetResourceObject(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>(InTextureName));
	Brush.ImageSize = InImageSize;
}

void FMinimapMarkerArray::Clear()
{
	Brush = FSlateBrush();

	for (UImage* Image : Array)
	{
		Owner->BackgroundPanel->RemoveChild(Image);
	}
	Array.SetNum(0);
	CurrLastVisibleWidgetIndex = -1;
	PrevLastVisibleWidgetIndex = -1;
}

UImage* FMinimapMarkerArray::Get()
{
	if (CurrLastVisibleWidgetIndex == Array.Num() - 1)
	{
		UImage* NewMarkerImage = NewObject<UImage>(Owner);
		NewMarkerImage->SetBrush(Brush);

		UCanvasPanelSlot* PanelSlot = Owner->BackgroundPanel->AddChildToCanvas(NewMarkerImage);
		PanelSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		PanelSlot->SetAutoSize(true);

		Array.Add(NewMarkerImage);
	}

	++CurrLastVisibleWidgetIndex;

	UImage* WidgetGet = Array[CurrLastVisibleWidgetIndex];
	WidgetGet->SetVisibility(ESlateVisibility::HitTestInvisible);
	return WidgetGet;
}

void FMinimapMarkerArray::Update()
{
	for (int32 i = CurrLastVisibleWidgetIndex + 1; i <= PrevLastVisibleWidgetIndex; ++i)
	{
		Array[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	PrevLastVisibleWidgetIndex = CurrLastVisibleWidgetIndex;
	CurrLastVisibleWidgetIndex = -1;
}
