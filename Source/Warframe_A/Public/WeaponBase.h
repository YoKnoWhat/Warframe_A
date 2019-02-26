// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


struct FWeaponFireMode
{
	float Accuracy;
	float ChargeRate;
	uint32 PelletCount;
	float CriticalChance; // Modded critical chance.
	float CriticalMultiplier;
	float FalloffStart;
	float FalloffEnd;
	float FalloffDamage;
	float FireRate;
	ENoiseType NoiseType;
	float PunchThrough;
	float ReloadTime;
	float StatusChance;
	EWeaponTriggerType TriggerType;
	TArray<FDamagePair> DamageArray;

	float BaseDamage;
	float TotalProportionalDamage;
	TArray<float> StatusChanceIntervalArray;
	uint32 CriticalTier;

	UPROPERTY()
	TArray<ARoundBase*> RoundPool;
};

UCLASS()
class WARFRAME_A_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetOwningCharacter(AWarframeCharacter *NewOwningCharacter);

	UFUNCTION(BlueprintCallable)
	void InitBP(int32 WeaponID, int32 Level/*Polarities, Mods*/);

	virtual void Init(EWeaponID WeaponID, uint32 Level/*Polarities, Mods*/);

	void ApplyMods();

	EDamageType GetStatusEffect(float Roll)const;

	void SetFireMode(uint32 FireMode);

	UFUNCTION(BlueprintCallable)
	void SetFireModeBP(int32 FireMode);

	UFUNCTION(BlueprintCallable)
	void BeginFire();

	// Allowing to call multiple times.
	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable)
	void BeginReload();

	UFUNCTION(BlueprintCallable)
	void StopReload();

	UFUNCTION(BlueprintCallable)
	bool IsReloading()const;

	// Property getters.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetName()const
	{
		return Name;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMaxAmmo()const
	{
		return AmmoMaximum;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMagazineSize()const
	{
		return MagazineCapacity;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetRemainingAmmo()const
	{
		return AmmoLeft;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetRemainingMagazine()const
	{
		return MagazineLeft;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAccuracy()const
	{
		return FireModeArray[CurrentFireMode].Accuracy;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetChargeRate()const
	{
		return FireModeArray[CurrentFireMode].ChargeRate;
	}

	FORCEINLINE uint32 GetPelletCount()const
	{
		return FireModeArray[CurrentFireMode].PelletCount;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCriticalTier()const
	{
		return FireModeArray[CurrentFireMode].CriticalTier;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCriticalChance()const
	{
		return FireModeArray[CurrentFireMode].CriticalChance;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCriticalMultiplier()const
	{
		return FireModeArray[CurrentFireMode].CriticalMultiplier;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetFalloffStart()const
	{
		return FireModeArray[CurrentFireMode].FalloffStart;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetFalloffEnd()const
	{
		return FireModeArray[CurrentFireMode].FalloffEnd;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetFalloffDamage()const
	{
		return FireModeArray[CurrentFireMode].FalloffDamage;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetFireRate()const
	{
		return FireModeArray[CurrentFireMode].FireRate;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetPunchThrough()const
	{
		return FireModeArray[CurrentFireMode].PunchThrough;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetReloadTime()const
	{
		return FireModeArray[CurrentFireMode].ReloadTime;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetStatusChance()const
	{
		return FireModeArray[CurrentFireMode].StatusChance;
	}

	FORCEINLINE EWeaponTriggerType GetTriggerType()const
	{
		return FireModeArray[CurrentFireMode].TriggerType;
	}

	FORCEINLINE const TArray<FDamagePair> &GetDamageArray()const
	{
		return FireModeArray[CurrentFireMode].DamageArray;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetBaseDamage()const
	{
		return FireModeArray[CurrentFireMode].BaseDamage;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetTotalProportionalDamage()const
	{
		return FireModeArray[CurrentFireMode].TotalProportionalDamage;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetBleedMultiplier()const
	{
		return BleedMultiplier;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHeatModMultiplier()const
	{
		return HeatModMultiplier;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetColdModMultiplier()const
	{
		return ColdModMultiplier;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetElectricityModMultiplier()const
	{
		return ElectricityModMultiplier;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetToxinModMultiplier()const
	{
		return ToxinModMultiplier;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetZoom()const
	{
		return Zoom;
	}

	// Called when a pellet is fired.
	UFUNCTION(BlueprintNativeEvent)
	ARoundBase *OnRoundFired(const FHitResult &CurrentTarget);
	ARoundBase *OnRoundFired_Implementation(const FHitResult &CurrentTarget);

private:
	// Fire the weapon using current fire mode.
	void Fire();

	bool Fire_Auto();

	bool Fire_SemiAuto();

	bool Fire_Charge();

	// Fire all pellets, then init thier attributes.
	void DoFire();

	void Reload();

protected:
	UPROPERTY()
	AWarframeCharacter *OwningCharacter;

	FName Name;
	EAmmoType AmmoType;
	uint32 MagazineCapacity;
	uint32 AmmoMaximum;

	float BleedMultiplier;
	float HeatModMultiplier;
	float ColdModMultiplier;
	float ElectricityModMultiplier;
	float ToxinModMultiplier;

	uint32 MagazineLeft;
	uint32 AmmoLeft;
	float MultishotChance;
	float Zoom;
	uint32 CurrentFireMode;
	TArray<FWeaponFireMode> FireModeArray;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;

	bool bIsReloading;

	// Cached variables.
	FVector TargetLocation;
};
