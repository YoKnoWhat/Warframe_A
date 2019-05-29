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
	float FireInterval;
	ENoiseType NoiseType;
	float PunchThrough;
	float ReloadTime; // todo: not in fire mode !
	float StatusChance;
	EWeaponTriggerType TriggerType;
	class FTriggerModifier* TriggerModifier;
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

	friend class FWeaponFactory;

public:	
	// Sets default values for this actor's properties
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay()override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetLevel(uint32 InLevel);

	/** Override this method to provide the correct class of derived weapon. */
	UFUNCTION(BlueprintNativeEvent)
	UClass* GetRoundClass()const;
	virtual UClass* GetRoundClass_Implementation()const;

	// Called when a pellet is fired.
	UFUNCTION(BlueprintNativeEvent)
	ARoundBase* OnRoundFired(const FHitResult &CurrentTarget);
	ARoundBase* OnRoundFired_Implementation(const FHitResult &CurrentTarget);

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

	/**
	 * Fire all pellets, then init thier attributes.
	 * Should only called by FTriggerModifier.
	 */
	void FireRound(float DamageScalar = 1.0f);

	UFUNCTION(BlueprintCallable)
	void BeginReload();

	UFUNCTION(BlueprintCallable)
	void StopReload();

	UFUNCTION(BlueprintCallable)
	bool IsReloading()const;

	void GainMagazine(uint32 Value);

	void ConsumeMagazine(uint32 Value);

	void GainAmmo(uint32 Value);

	// Property getters.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetName()const
	{
		return Name;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetLevel()const
	{
		return Level;
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
	FORCEINLINE float GetFireInterval()const
	{
		return FireModeArray[CurrentFireMode].FireInterval;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ENoiseType GetNoiseType()const
	{
		return FireModeArray[CurrentFireMode].NoiseType;
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

	FORCEINLINE class USkeletalMeshComponent* GetMesh()const
	{
		return SkeletalMeshComponent;
	}

	FORCEINLINE void SetFireEmitter(class UParticleSystem* InFireEmitter)
	{
		FireEmitter = InFireEmitter;
	}

	FORCEINLINE void SetOnHitEmitter(class UNiagaraSystem* InOnHitEmitter)
	{
		OnHitEmitter = InOnHitEmitter;
	}

	FORCEINLINE class UNiagaraSystem* GetOnHitEmitter()
	{
		return OnHitEmitter;
	}

protected:
	/** Override this method to init the extra properties of derived weapon. */
	virtual void Init(EWeaponID WeaponID/*Polarities, Mods*/);

	class FTriggerModifier* InitTriggerModifier(const struct FWeaponModeInfo& ModeInfo);

	void Reload();

	FORCEINLINE class FTriggerModifier* GetTriggerModifier()const
	{
		return FireModeArray[CurrentFireMode].TriggerModifier;
	}

protected:
	/** Static or skeletal mesh component instance determined by overrider. */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY()
	class UParticleSystem* FireEmitter;

	UPROPERTY()
	class UNiagaraSystem* OnHitEmitter;

	FName Name;
	uint32 Level;
	EAmmoType AmmoType;
	uint32 MagazineCapacity;
	uint32 AmmoMaximum;
	TArray<FWeaponFireMode> FireModeArray;

	/** Mutable variables. */
	float BleedMultiplier;
	float HeatModMultiplier;
	float ColdModMultiplier;
	float ElectricityModMultiplier;
	float ToxinModMultiplier;
	float MultishotChance;
	float Zoom;

	/** Used with FWeaponFireMode::TriggerSpecifiedData. */
	uint32 MagazineLeft;
	uint32 AmmoLeft;
	uint32 CurrentFireMode;

	float TimeSinceLastFire;
	float TimeSinceReloadBegin;

	bool bIsFiring;
	bool bIsReloading;

	// Cached variables.
	FVector TargetLocation;
};
