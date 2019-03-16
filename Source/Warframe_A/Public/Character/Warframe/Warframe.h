// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCharacter.h"
#include "Warframe.generated.h"


UCLASS(BlueprintType)
class WARFRAME_A_API AWarframe : public AWarframeCharacter
{
	GENERATED_BODY()

protected:
	struct FAbilityInfo
	{
		bool CanCastInAir;
		bool CanRecast;
		float PreCastTime;
		float PostCastTime;

		/** Mutable variables. */
		float CastProgress;
	};

public:
	AWarframe(const FObjectInitializer& ObjectInitializer);

	~AWarframe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Init(ECharacterID CharacterID, uint32 Level)override;

	virtual void OnLevelChanged();

	void GainEnergy(float Value);

	UFUNCTION(BlueprintCallable)
	bool CastAbilityBP(int32 AbilityIndex, float Charge);

	bool CastAbility(uint32 AbilityIndex, float Charge);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMaxEnergy()const
	{
		return MaxEnergy;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAbilityStrength()const
	{
		return AbilityStrength;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAbilityRange()const
	{
		return AbilityRange;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAbilityEfficiency()const
	{
		return AbilityEfficiency;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAbilityDuration()const
	{
		return AbilityDuration;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetSlideDeceleration()const
	{
		return SlideDeceleration;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentEnergy()const
	{
		return CurrentEnergy;
	}

protected:
	void TryFindTarget();

protected:
	uint32 MaxEnergy;
	float AbilityStrength;
	float AbilityRange;
	float AbilityEfficiency;
	float AbilityDuration;
	float SlideDeceleration = -1.0f;

	/** Allocated in derived class, deleted in base class. */
	FAbilityObject *Abilities[4];

	uint32 CurrentEnergy;
};
