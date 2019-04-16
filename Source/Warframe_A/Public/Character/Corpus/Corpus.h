// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCharacter.h"
#include "Corpus.generated.h"


UCLASS(BlueprintType)
class WARFRAME_A_API ACorpus : public AWarframeCharacter
{
	GENERATED_BODY()

public:
	ACorpus(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Init(ECharacterID InCharacterID)override;
};