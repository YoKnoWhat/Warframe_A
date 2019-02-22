// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "GameFramework/Actor.h"
#include "RoundBase.generated.h"

UCLASS()
class WARFRAME_A_API ARoundBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitBP(AWeaponBase* Weapon);

	virtual void Init(AWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable)
	void OnHit(AActor *Target, FVector HitLocation);

	// Property getters.
	FORCEINLINE float GetBaseDamage()const
	{
		return BaseDamage;
	}

	FORCEINLINE float GetBaseDamagePhys()const
	{
		return BaseDamagePhys;
	}

	FORCEINLINE const TArray<FDamagePair> &GetDamageArray()const
	{
		return DamageArray;
	}

	FORCEINLINE uint32 GetCriticalTier()const
	{
		return CriticalTier;
	}

	FORCEINLINE EDamageType GetStatusEffect()const
	{
		return StatusEffect;
	}

protected:
	// Basic attributes inherited from shooter.
	float CriticalMultiplier;
	float FalloffStart;
	float FalloffEnd;
	float FalloffDamage;
	float PunchThrough;
	float BaseDamage; // Physical and elemental damage.
	float BaseDamagePhys;
	TArray<FDamagePair> DamageArray;

	// Extra attributes computed in Init().
	uint32 CriticalTier;
	EDamageType StatusEffect;

	UPROPERTY(BlueprintReadOnly)
	FVector LastLocation;

	float FliedDistance;
};
