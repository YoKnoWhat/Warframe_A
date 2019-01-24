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
	
	virtual void Init();

	const FCharacterProperties *GetCharacterProp(ECharacterID CharacterID)const;

	const FName GetCharacterName(ECharacterID CharacterID)const;

protected:
	TMap<ECharacterID, FCharacterProperties> CharacterPropTable;
};
