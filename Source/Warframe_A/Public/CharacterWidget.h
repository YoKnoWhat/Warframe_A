// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "Blueprint/UserWidget.h"
#include "WarframeCharacter.h"
#include "CharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARFRAME_A_API UCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCharacterWidget(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy()override;

	void SetWarframeCharacter(AWarframeCharacter *WarframeCharacter);

	void OnSelected();

	void OnUnselected();

protected:
	/**
	* Play fade out animation, and remove the widget from VisibleWidgetArray when it is done.
	*/
	void FadeOut();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	AWarframeCharacter* WarframeCharacter;

	// At most 3 widgets are visible at the same time.
	// widget with index 0 is the oldest visible one.
	static TArray<UCharacterWidget*> VisibleWidgetArray;

	// Current render opacity of the widget.
	float CurrentOpacity;

	// Delta to subtract from CurrentOpactiy when FadeOut() is called.
	float OpacityStep;

	// Timer handle.
	FTimerHandle FadeOutTimer;
};
