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

	virtual void SetVisibility(ESlateVisibility InVisibility)override;

	virtual void NativeConstruct()override;

	virtual void NativeDestruct()override;

	virtual bool Initialize()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

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
	UPROPERTY(Meta=(BindWidget))
	UProgressBar*	HeaderBar;
	UPROPERTY(Meta = (BindWidget))
	UImage*			HealthDrop;
	UPROPERTY(Meta = (BindWidget))
	UImage*			ShieldDrop;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock*		Name;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Slash;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Impact;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Puncture;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Heat;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Cold;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Electricity;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Toxin;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Blast;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Radiation;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Gas;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Magnetic;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Viral;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Corrosive;
	UPROPERTY(Meta = (BindWidget))
	UImage*			Void;
	UPROPERTY(Meta = (BindWidget))
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
