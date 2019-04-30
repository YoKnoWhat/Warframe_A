// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "SpawnPoint.generated.h"


USTRUCT()
struct FSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 CharacterID;

	UPROPERTY(EditAnywhere)
	int32 Level;

	UPROPERTY(EditAnywhere)
	int32 Number;
};

UCLASS()
class WARFRAME_A_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnPoint(const FObjectInitializer& ObjectInitializer);

	void SpawnAnyway();

	void SpawnIfEmpty();

	void KillAll();

	bool IsEmpty()const;

protected:
	virtual void BeginPlay()override;

	UFUNCTION()
	void OnCharacterDied(AActor* Causer, AWarframeCharacter* Character);
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<FSpawnInfo> SpawnInfoArray;

	UPROPERTY()
	TSet<AWarframeCharacter*> SpawnedCharacters;
};
