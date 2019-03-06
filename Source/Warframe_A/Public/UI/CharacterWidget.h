// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "Blueprint/UserWidget.h"
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

	void SetWarframeCharacter(AWarframeCharacter *WarframeCharacter);

	virtual void Init();

	virtual void OnSelected();

	virtual void OnUnselected();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	AWarframeCharacter* WarframeCharacter;
};
