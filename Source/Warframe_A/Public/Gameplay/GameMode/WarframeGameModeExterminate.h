// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/GameMode/WarframeGameMode.h"
#include "WarframeGameModeExterminate.generated.h"

/**
 * 
 */
UCLASS()
class WARFRAME_A_API AWarframeGameModeExterminate : public AWarframeGameMode
{
	GENERATED_BODY()
	
public:
	FORCEINLINE int32 GetCurrentCounter()const
	{
		return CurrentCounter;
	}

	FORCEINLINE int32 GetTargetCounter()const
	{
		return TargetCounter;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnCharacterDied(AActor *Causer, AWarframeCharacter* Character)override;

protected:
	uint32 CurrentCounter;
	uint32 TargetCounter;
};
