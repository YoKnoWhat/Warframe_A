// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "GameFramework/GameMode.h"
#include "WarframeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WARFRAME_A_API AWarframeGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	// $DamageCauser could be nullptr!
	UFUNCTION(BlueprintNativeEvent)
	void OnCharacterDamaged(AActor* DamageCauser, AWarframeCharacter* DamagedCharacter, FVector HitLocation, EDamageType StatusEffect, float Damage, bool IsDamageOnShield, int32 CriticalTier);
	void OnCharacterDamaged_Implementation(AActor* DamageCauser, AWarframeCharacter* DamagedCharacter, FVector HitLocation, EDamageType StatusEffect, float Damage, bool IsDamageOnShield, int32 CriticalTier);

	void OnCharacterSpawned(AWarframeCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterSpawnedBP(AWarframeCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterDied(AActor *Causer, AWarframeCharacter* Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Handles second half of RestartPlayer */
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)override;

	void ReadInSpawnPointArray();

public:
	// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSpawnedDelegate, AWarframeCharacter*, NewCharacter);
	// UPROPERTY(BlueprintAssignable)
	// FOnCharacterSpawnedDelegate OnCharacterSpawned;

protected:
	UPROPERTY()
	TArray<USpawnPoint*> SpawnPointArray;
};
