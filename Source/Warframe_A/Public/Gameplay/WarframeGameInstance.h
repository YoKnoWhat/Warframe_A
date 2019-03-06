// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "Engine/GameInstance.h"
#include "WarframeGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class WARFRAME_A_API UWarframeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UWarframeGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init()override;

	virtual void Shutdown()override;

	const FCharacterInfo* GetCharacterInfo(ECharacterID CharacterID)const;

	const FCharacterAppearance* GetCharacterAppearance(ECharacterID CharacterID)const;

	const FEnemyInfo* GetEnemyInfo(ECharacterID CharacterID)const;

	const FWarframeInfo* GetWarframeInfo(ECharacterID CharacterID)const;

	const FWeaponInfo* GetWeaponInfo(EWeaponID WeaponID)const;

protected:
	void ReadInCharacterTable(const char* Begin, const char* End);

	void ReadInCharacterAppearanceTable(const char* Begin, const char* End);

	void ReadInEnemyTable(const char* Begin, const char* End);

	void ReadInWarframeTable(const char* Begin, const char* End);

	void ReadInWeaponTable(const char* Begin, const char* End);

protected:
	TArray<FCharacterInfo> CharacterInfoArray;

	TArray<FCharacterAppearance> CharacterAppearanceArray;

	TArray<FEnemyInfo> EnemyInfoArray;

	TArray<FWarframeInfo> WarframeInfoArray;

	TArray<FWeaponInfo> WeaponInfoArray;
};
