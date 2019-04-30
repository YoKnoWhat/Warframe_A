// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/GameMode/WarframeGameModeExterminate.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/SpawnPoint.h"
#include "Gameplay/WarframeGameInstance.h"
#include "UI/PlayerHUD.h"
#include "Utility/HelperFunction.h"

#include "Runtime/Core/Public/Misc/Paths.h"


void AWarframeGameModeExterminate::BeginPlay()
{
	Super::BeginPlay();

	CurrentCounter = 0;
	TargetCounter = 5;

	if (Player != nullptr)
	{
		HUD->AddMission(EMissionType::Exterminate);
	}
}

void AWarframeGameModeExterminate::OnCharacterDied(AActor *Causer, AWarframeCharacter* Character)
{
	Super::OnCharacterDied(Causer, Character);

	if (++CurrentCounter == TargetCounter)
	{
		this->RestartGame();
		// Play countdown animation.
		// Freeze controller.
		// Press 'R' to restart game.
	}
}
