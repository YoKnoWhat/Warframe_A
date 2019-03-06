// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterWidget.h"

#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "CharacterHeaderPanel.generated.h"


/**
 * Character header panel: name, statuses, header bar, level.
 */
UCLASS()
class WARFRAME_A_API UCharacterHeaderPanel : public UCharacterWidget
{
	GENERATED_BODY()

public:
	UCharacterHeaderPanel(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	virtual void Init()override;

	virtual void OnSelected()override;

	virtual void OnUnselected()override;

protected:
	/**
	* Play fade out animation, and remove the widget from VisibleWidgetArray when it is done.
	*/
	void FadeOut();

	void HealthDropFadeOut();

	void ShieldDropFadeOut();

	void UpdateHeaderBar();

	void UpdateStatusImages();

protected:
	UProgressBar*	HeaderBar;
	UImage*			HealthDrop;
	UImage*			ShieldDrop;
	UTextBlock*		Name;
	UImage*			Slash;
	UImage*			Impact;
	UImage*			Puncture;
	UImage*			Heat;
	UImage*			Cold;
	UImage*			Electricity;
	UImage*			Toxin;
	UImage*			Blast;
	UImage*			Radiation;
	UImage*			Gas;
	UImage*			Magnetic;
	UImage*			Viral;
	UImage*			Corrosive;
	UImage*			Void;
	UTextBlock*		Level;

	// At most 3 widgets are visible at the same time.
	// widget with index 0 is the oldest visible one.
	static TArray<UCharacterHeaderPanel*> VisibleWidgetArray;

	// Dynamic material of the widget.
	UMaterialInstanceDynamic* DynamicMaterial;

	// Current render opacity of the widget.
	float CurrentOpacity;

	// Delta to subtract from CurrentOpactiy when FadeOut() is called.
	float OpacityStep;

	// Current opacity of health and shield drop image.
	float HealthDropOpacity;
	float ShieldDropOpacity;

	float HealthPercentCache;
	float ShieldPercentCache;

	// Timer handle.
	FTimerHandle FadeOutTimer;
	FTimerHandle HealthDropTimer;
	FTimerHandle ShieldDropTimer;
};
