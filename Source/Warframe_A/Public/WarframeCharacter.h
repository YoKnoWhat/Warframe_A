// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "GameFramework/Character.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "Runtime/Core/Public/Containers/Queue.h"
#include "WarframeCharacter.generated.h"


struct FStatusEffectData
{
	EDamageType Type;
	AActor *DamageCauser;
	FVector HitLocation;

	// Damage per tick.
	float Damage;

	// Remained count of ticks.
	uint32 TickCount;

	// Time that this status effect should tick.
	float TickTime;

	// For AWarframeCharacter::StatusEffectSet() only.
	friend inline bool operator==(const FStatusEffectData& LHS, const FStatusEffectData& RHS)
	{
		return LHS.Type == RHS.Type;
	}

	// For AWarframeCharacter::StatusEffectSet() only.
	friend inline uint32 GetTypeHash(const FStatusEffectData& Other)
	{
		return GetTypeHash(CastToUnderlyingType(Other.Type));
	}
};

UCLASS()
class WARFRAME_A_API AWarframeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWarframeCharacter(const FObjectInitializer &ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void InitPropertiesBP(int32 CharacterID, int32 Level);

	virtual void Init(ECharacterID CharacterID, uint32 Level);

	const FHitResult &GetTarget()const;

	UFUNCTION(BlueprintCallable)
	void ApplyDamageBP(AActor *DamageCauser, EDamageType Status, EDamageType DamageType, float Damage);

	void ApplyDamage(AActor *DamageCauser, const FVector &HitLocation, EDamageType DamageType, float Damage);

	// No Raw damage applied assumed.
	void ApplyDamage(AActor *DamageCauser, const FVector &HitLocation, ARoundBase *Round, float DamageScalar);

	// Property getters.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetLevel()const
	{
		return this->Level;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetName()const
	{
		return this->Name;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EHealthType GetHealthType()const
	{
		return this->HealthType;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth()const
	{
		return this->MaxHealth;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHealth()const
	{
		return this->CurrentHealth;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EShieldType GetShieldType()const
	{
		return this->ShieldType;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxShield()const
	{
		return this->MaxShield;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentShield()const
	{
		return this->CurrentShield;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EArmorType GetArmorType()const
	{
		return this->ArmorType;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetArmor()const
	{
		return this->Armor;
	}

	UFUNCTION(BlueprintCallable)
	AWeaponBase *GetWeapon(EWeaponType WeaponType);

	UFUNCTION(BlueprintCallable)
	AWeaponBase *GetCurrentWeapon();

	// Multiplier getters.
	FORCEINLINE float GetMovementSpeedMultiplier()
	{
		return this->MovementSpeedMultiplier;
	}

protected:
	float PropertyLevelScaling(float BaseValue, float BaseLevel, float Exponent, float Coefficient, float CurrentLevel);

	void ApplyStatusEffect(AActor *DamageCauser, const FVector &HitLocation, EDamageType Status, float BaseDamage, float BaseDamagePhys);

	// Status effect tick functions.
	void SlashStatusTick(const FStatusEffectData &Data);

	void ImpactStatusTick(const FStatusEffectData &Data);

	void PunctureStatusTick(const FStatusEffectData &Data);

	void HeatStatusTick(const FStatusEffectData &Data);

	void ColdStatusTick(const FStatusEffectData &Data);

	void ElectricityStatusTick(const FStatusEffectData &Data);

	void ToxinStatusTick(const FStatusEffectData &Data);

	void BlastStatusTick(const FStatusEffectData &Data);

	void RadiationStatusTick(const FStatusEffectData &Data);

	void GasStatusTick(const FStatusEffectData &Data);

	void MagneticStatusTick(const FStatusEffectData &Data);

	void ViralStatusTick(const FStatusEffectData &Data);

	void CorrosiveStatusTick(const FStatusEffectData &Data);

	void VoidStatusTick(const FStatusEffectData &Data);

	void NoneStatusTick(const FStatusEffectData &Data);

	float GetHealthDamageModifier(EDamageType DamageType)const;

	float GetShieldDamageModifier(EDamageType DamageType)const;

	float GetArmorDamageModifier(EDamageType DamageType)const;

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FCharacterOnDiedDelegate, AActor* /*Causer*/, AWarframeCharacter* /*DeadCharacter*/);
	FCharacterOnDiedDelegate OnDied;

protected:
	UPROPERTY(VisibleAnywhere)
	UCharacterWidgetComponent *CharacterWidget;

	uint32 Level;
	FName Name;
	EHealthType HealthType;
	float MaxHealth;
	float CurrentHealth;
	EShieldType ShieldType;
	float MaxShield;
	float CurrentShield;
	EArmorType ArmorType;
	float Armor;
	float Affinity;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AWeaponBase *EquippedWeapon;

	UPROPERTY()
	FHitResult CurrentTarget;

	float InternalTime;
	TQueue<FStatusEffectData> StatusEffectQueue; // Slash, Heat, Toxin, Gas.
	TSet<FStatusEffectData> StatusEffectSet; // Impact, Puncture, Cold, Electricity, Blast, Radiation, Magnetic, Viral, Corrosive, Void.
	FStatusEffectData *SlashStatusData;
	FStatusEffectData *ImpactStatusData;
	FStatusEffectData *PunctureStatusData;
	FStatusEffectData *HeatStatusData;
	FStatusEffectData *ColdStatusData;
	FStatusEffectData *ElectricityStatusData;
	FStatusEffectData *ToxinStatusData;
	FStatusEffectData *BlastStatusData;
	FStatusEffectData *RadiationStatusData;
	FStatusEffectData *GasStatusData;
	FStatusEffectData *MagneticStatusData;
	FStatusEffectData *ViralStatusData;
	FStatusEffectData *CorrosiveStatusData;
	FStatusEffectData *VoidStatusData;

	float ShieldRechargeDelayMultiplier = 1.0f;
	float ShieldRechargeSpeedMultiplier = 1.0f;
	float ShieldRechargeTimer = 0.0f;
	float MovementSpeedMultiplier = 1.0f;
	float HeatModeMultiplier = 1.0f;
};
