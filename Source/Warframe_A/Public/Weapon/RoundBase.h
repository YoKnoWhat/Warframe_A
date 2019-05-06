// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "GameFramework/Actor.h"
#include "RoundBase.generated.h"

/**
 * Override Tick() or OnBeginOverlap to implement different round logic.
 */
UCLASS()
class WARFRAME_A_API ARoundBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoundBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay()override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime)override;

	UFUNCTION(BlueprintCallable)
	void InitBP(AWeaponBase* Weapon);

	virtual void Init(AWeaponBase* Weapon);

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * Call this method to apply damage to target(with attitude check).
	 * The round will not be destroyed.
	 */
	void OnHit(AActor *Target, FVector HitLocation, FName BoneName);

	// Property getters.
	FORCEINLINE class UShapeComponent* GetShape()const
	{
		return ShapeComponent;
	}

	FORCEINLINE class UMeshComponent* GetMesh()const
	{
		return MeshComponent;
	}

	FORCEINLINE class UMovementComponent* GetMovement()const
	{
		return MovementComponent;
	}

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

	FORCEINLINE void ApplyDamageScalar(float Scalar)
	{
		DamageScalar *= Scalar;
	}

	FORCEINLINE float GetDamageScalar()const
	{
		return DamageScalar;
	}

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UShapeComponent* ShapeComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UMovementComponent* MovementComponent;

	UPROPERTY()
	class UNiagaraSystem* OnHitEmitter;

	// Basic attributes inherited from shooter.
	float CriticalChance; // [0-100]
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
	float DamageScalar; // Damage scalar due to charging, critical hit and falloff .
	float StatusDamageMultiplier; // Status damage multiplier.

	UPROPERTY(BlueprintReadOnly)
	FVector LastLocation;

	float FliedDistance;
};
