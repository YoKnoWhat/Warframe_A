// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/MinimapMarkerArray.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"
#include "Runtime/UMG/Public/Components/CanvasPanelSlot.h"
#include "Minimap.generated.h"


UCLASS()
class WARFRAME_A_API UMinimap : public UUserWidget
{
	GENERATED_BODY()

	friend struct FMinimapMarkerArray;

public:
	UMinimap(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void Init(const FVector& InLevelOrigin, float InUnitScalar = 0.05f);

	UFUNCTION(BlueprintCallable)
	void Update(class AWarframe* Character);

	UFUNCTION(BlueprintCallable)
	void ClearCache();

protected:
	FVector LevelOrigin;
	float UnitScalar;

	UPROPERTY(Meta = (BindWidget))
		UImage*			PlayerMarker;
	UPROPERTY(Meta = (BindWidget))
		UCanvasPanel*	BackgroundPanel;
	UPROPERTY(Meta = (BindWidget))
		UImage*			BackgroundImage;

	FMinimapMarkerArray EnemyMarkerArray;
	FMinimapMarkerArray ResourceMarkerArray;
};
