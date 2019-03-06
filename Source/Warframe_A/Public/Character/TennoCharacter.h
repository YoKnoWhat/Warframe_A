// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCharacter.h"
#include "TennoCharacter.generated.h"

UCLASS(BlueprintType)
class WARFRAME_A_API ATennoCharacter : public AWarframeCharacter
{
	GENERATED_BODY()

public:
	ATennoCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Init(ECharacterID CharacterID, uint32 Level)override;

protected:
	void TryFindTarget();
};
