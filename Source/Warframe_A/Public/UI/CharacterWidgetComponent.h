// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "Components/WidgetComponent.h"
#include "CharacterWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARFRAME_A_API UCharacterWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
		
public:
	UCharacterWidgetComponent(const FObjectInitializer &ObjectInitializer);

	virtual void InitWidget()override;
};
