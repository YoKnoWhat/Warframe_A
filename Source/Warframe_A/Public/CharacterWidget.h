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
	void SetWarframeCharacter(AWarframeCharacter *WarframeCharacter);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	AWarframeCharacter* WarframeCharacter;
};
