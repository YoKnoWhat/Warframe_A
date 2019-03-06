// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "UObject/NoExportTypes.h"
#include "SpawnPoint.generated.h"


struct FCharacterSpawnInfo
{
	ECharacterID CharacterID;
	uint32 Level;
	uint32 Number;
};

UCLASS()
class WARFRAME_A_API USpawnPoint : public UObject
{
	GENERATED_BODY()
	
public:	
	void Init(const FVector &Location, const TArray<FCharacterSpawnInfo> &SpawnInfoArray);

	void SpawnAnyway();

	void SpawnIfAllKilled();

	void KillAll();

protected:
	UFUNCTION()
	void OnCharacterDied(AActor* Causer, AWarframeCharacter* Character);
	
protected:
	FVector Location;
	TArray<FCharacterSpawnInfo> SpawnInfoArray;

	UPROPERTY()
	TSet<AWarframeCharacter*> SpawnedCharacters;
};
