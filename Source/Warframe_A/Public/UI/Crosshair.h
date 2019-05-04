// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Crosshair.generated.h"


UCLASS()
class WARFRAME_A_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()

public:
	UCrosshair(const FObjectInitializer& ObjectInitializer);

	void Update(class AWarframe* Character);

	void OnApplyDamageToEnemy(bool IsHeadShot);

protected:
	bool bCacheIsAiming;
	
	/** Focus widgets begin. */
	UPROPERTY(Meta = (BindWidget))
		UImage* Focus_Dot;
	UPROPERTY(Meta = (BindWidget))
		UImage* Focus_Normal;
	UPROPERTY(Meta = (BindWidget))
		UImage* Focus_Left;
	UPROPERTY(Meta = (BindWidget))
		UImage* Focus_Right;
	/** Focus widgets end. */

	/** HitMark widgets begin. */
	UPROPERTY(Meta = (BindWidget))
		UImage* HitMark_TopRight;
	UPROPERTY(Meta = (BindWidget))
		UImage* HitMark_TopLeft;
	UPROPERTY(Meta = (BindWidget))
		UImage* HitMark_BottomRight;
	UPROPERTY(Meta = (BindWidget))
		UImage* HitMark_BottomLeft;
	/** HitMark widgets end. */

	/** Widget animations begin. */
	UPROPERTY(Meta = (BindWidgetAnim))
		UWidgetAnimation* FocusAnimation;
	UPROPERTY(Meta = (BindWidgetAnim))
		UWidgetAnimation* NormalHitAnimation;
	UPROPERTY(Meta = (BindWidgetAnim))
		UWidgetAnimation* HeadShotHitAnimation;
	/** Widget animations end. */
};
