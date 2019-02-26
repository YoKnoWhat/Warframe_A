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

	FORCEINLINE const TArray<FDamagePair> &GetDamageArray()const
	{
		return DamageArray;
	}

	FORCEINLINE float GetBleedMultiplier()const
	{
		return BleedMultiplier;
	}

	FORCEINLINE float GetHeatModMultiplier()const
	{
		return HeatModMultiplier;
	}

	FORCEINLINE float GetColdModMultiplier()const
	{
		return ColdModMultiplier;
	}

	FORCEINLINE float GetElectricityModMultiplier()const
	{
		return ElectricityModMultiplier;
	}

	FORCEINLINE float GetToxinModMultiplier()const
	{
		return ToxinModMultiplier;
	}

	FORCEINLINE uint32 GetCriticalTier()const
	{
		return CriticalTier;
	}

	FORCEINLINE EDamageType GetStatusEffect()const
	{
		return StatusEffect;
	}

	FORCEINLINE float GetDamageScalar()const
	{
		return DamageScalar;
	}

protected:
	// Basic attributes inherited from shooter.
	float CriticalMultiplier;
	float FalloffStart;
	float FalloffEnd;
	float FalloffDamage;
	float PunchThrough;
	float BaseDamage;
	TArray<FDamagePair> DamageArray;
	float BleedMultiplier;
	float HeatModMultiplier;
	float ColdModMultiplier;
	float ElectricityModMultiplier;
	float ToxinModMultiplier;

	// Extra attributes computed in Init().
	uint32 CriticalTier;
	EDamageType StatusEffect;
	float DamageScalar; // Damage scalar due to critical hit and falloff.
	float StatusDamageMultiplier; // Status damage multiplier.

	UPROPERTY(BlueprintReadOnly)
	FVector LastLocation;

	float FliedDistance;
};
