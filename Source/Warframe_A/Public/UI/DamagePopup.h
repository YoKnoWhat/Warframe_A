// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "Components/HorizontalBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "DamagePopup.generated.h"

/**
 * 
 */
UCLASS()
class WARFRAME_A_API UDamagePopup : public UHorizontalBox
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(UUserWidget* HUD);

	UFUNCTION(BlueprintCallable)
	void Show(const FVector &HitLocation, EDamageType Status, float Damage, bool IsDamageOnShield, int32 CriticalTier);

	UFUNCTION(BlueprintCallable)
	void Hide();

	UFUNCTION(BlueprintCallable)
	bool Update(float DeltaTime);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* CachedHUD;

	UPROPERTY()
	UTextBlock* DamageText;

	UPROPERTY()
	UImage* StatusImage;

	float Time;
	float InitScale;
	FVector HitLocation;
	FVector2D ControlPointOffset0;
	FVector2D ControlPointOffset1;

	// Temporary configurable variables.
	UPROPERTY(EditAnywhere)
		float MinRandomDist = 100.0f;
	UPROPERTY(EditAnywhere)
		float MaxRandomDist = 1500.0f;
	UPROPERTY(EditAnywhere)
		float MaxRandomY = 500.0f;
};
