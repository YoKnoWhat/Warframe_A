// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "UObject/NoExportTypes.h"
#include "WarframeConfigSingleton.generated.h"

/**
 * Override this class in Blueprint to config game classes used.
 */
UCLASS(Blueprintable)
class WARFRAME_A_API UWarframeConfigSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UWarframeConfigSingleton(const FObjectInitializer& ObjectInitializer);

	static UWarframeConfigSingleton *Instance();

public:
	UPROPERTY(EditAnywhere, Category = UserInterface)
		TSubclassOf<UCharacterWidget> CharacterWidgetClass;
	
	
};
