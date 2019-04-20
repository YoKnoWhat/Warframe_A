
#pragma once

#include "Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "MinimapMarkerArray.generated.h"


/** Forward declaration. */
class UMinimap;

USTRUCT()
struct WARFRAME_A_API FMinimapMarkerArray
{
	GENERATED_BODY();

public:
	FMinimapMarkerArray();

	/** Init owner and brush. */
	void Init(UMinimap* InOwner, const FName& InTextureName, const FVector2D& InImageSize);

	/** Slow operation. */
	void Clear();

	/** Return image widget with visiblity HitTestInvisible. */
	UImage* Get();

	/** Called each time when Minimap updates. */
	void Update();

private:
	UMinimap* Owner;

	UPROPERTY()
	FSlateBrush Brush;

	TArray<UImage*> Array;
	int32 CurrLastVisibleWidgetIndex = -1;
	int32 PrevLastVisibleWidgetIndex = -1;
};
