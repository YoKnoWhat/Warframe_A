// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "GameFramework/Character.h"
#include "ObjectPool.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "Runtime/Core/Public/Containers/Queue.h"
#include "WarframeCharacter.generated.h"


struct FStatusEffectData
{
	EDamageType Type;
	AActor *DamageCauser;
	FVector HitLocationOffset;

	// Damage per tick.
	float Damage;

	// Remained count of status ticks such as Slash, Heat, etc.
	// For status like Impact, Puncture, etc., the count will always be 0.
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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void InitPropertiesBP(int32 CharacterID, int32 Level);

	virtual void Init(ECharacterID CharacterID, uint32 Level);

	// Selected by player.
	void OnSelected();

	// Unselected by player.
	void OnUnselected();

	const FHitResult &GetSelectedTarget()const;

	void SetOrbDropChances(float HealthOrbChance, float EnergyOrbChance);

	void SetCommonDropItems(const TArray<EPickableObjectID>& IDs, const TArray<float>& Chances);

	UFUNCTION(BlueprintCallable)
	void ApplyDamageBP(AActor *DamageCauser, EDamageType Status, EDamageType DamageType, float Damage);

	void ApplyDamage(AActor* DamageCauser, const FVector& HitLocation, EDamageType DamageType, float Damage);

	/**
	 * Apply damage to the character.
	 * @param Status	Used to notify game mode what status effect is applied to the character when it get damaged.
	 */
	void ApplyDamage(AActor* DamageCauser, const FVector& HitLocation, EDamageType Status, TArray<FDamagePair> &DamageArray, float DamageScalar, uint32 CriticalTier);

	/**
	 * Apply specific status effect to the character.
	 * @param DamageMultiplier	The damage multiplier used by status that dealing damage when ticking.
	 */
	void ApplyStatusEffect(AActor* DamageCauser, const FVector& HitLocation, EDamageType Status, float BaseDamage, float DamageMultiplier);

	UFUNCTION(BlueprintCallable)
	float GetStatusTime(EDamageType Type)const;

	// Property getters.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStateMachineComponent* GetStateMachine()const
	{
		return this->StateMachineComponent;
	}

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

	void DropItem();

	// Status effect tick functions.
	void SlashStatusTick(const FStatusEffectData* Data);

	void ImpactStatusTick(const FStatusEffectData* Data);

	void PunctureStatusTick(const FStatusEffectData* Data);

	void HeatStatusTick(const FStatusEffectData* Data);

	void ColdStatusTick(const FStatusEffectData* Data);

	void ElectricityStatusTick(const FStatusEffectData* Data);

	void ToxinStatusTick(const FStatusEffectData* Data);

	void BlastStatusTick(const FStatusEffectData* Data);

	void RadiationStatusTick(const FStatusEffectData* Data);

	void MagneticStatusTick(const FStatusEffectData* Data);

	void ViralStatusTick(const FStatusEffectData* Data);

	void CorrosiveStatusTick(const FStatusEffectData* Data);

	void VoidStatusTick(const FStatusEffectData* Data);

	void NoneStatusTick(const FStatusEffectData* Data);

	float GetHealthDamageModifier(EDamageType DamageType)const;

	float GetShieldDamageModifier(EDamageType DamageType)const;

	float GetArmorDamageModifier(EDamageType DamageType)const;

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FCharacterOnDiedDelegate, AActor* /*Causer*/, AWarframeCharacter* /*DeadCharacter*/);
	FCharacterOnDiedDelegate OnDied;

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCharacterWidgetComponent* CharacterWidgetComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStateMachineComponent* StateMachineComponent;

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

	struct FItemDropPair
	{
		EPickableObjectID ID;
		float Chance;
	};
	TArray<FItemDropPair> OrbDropList;
	TArray<FItemDropPair> CommonItemDropList;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AWeaponBase *EquippedWeapon;

	UPROPERTY(BlueprintReadOnly)
	FHitResult SelectedTarget;

	float InternalTime;

	static FObjectPool<FStatusEffectData> StatusEffectDataPool;

	/* Slash, Toxin, Gas status instances are placed here.
	* These statuses have the same tick interval. When a status, which instances with the EDamageType can be found in the queue, is applied
	* to the character, it will not force the existed instance to refresh its state. In contrast, the new instance will be placed at the
	* tail of the queue.
	*/
	TQueue<FStatusEffectData*> StatusEffectQueue;

	/* Impact, Puncture, Heat, Cold, Electricity, Blast, Radiation, Magnetic, Viral, Corrosive, Void status instances are placed here.
	* These statues have different tick interval. When a status, which instances with the EDamageType can be found in the set, is applied
	* to the character, it will refresh the state of existed status instance. That is, there will be only one instance of each type of
	* in the set.
	*/
	TSet<FStatusEffectData*> StatusEffectSet; 

	// Used to track the latest status instance for each type of status.
	TArray<FStatusEffectData*> LatestStatusEffectData;

	float ShieldRechargeDelayMultiplier = 1.0f;
	float ShieldRechargeSpeedMultiplier = 1.0f;
	float ShieldRechargeTimer = 0.0f;
	float MovementSpeedMultiplier = 1.0f;
	float HeatModeMultiplier = 1.0f;
};
