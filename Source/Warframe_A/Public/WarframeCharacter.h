// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "GameFramework/Character.h"
#include "WarframeCharacter.generated.h"


UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Impact,
	Puncture,
	Slash,
	Heat,
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

	virtual void InitProperties(ECharacterID CharacterID, uint32 Level);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetLevel()
	{
		return this->Level;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetName()
	{
		return this->Name;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EHealthType GetHealthType()
	{
		return this->HealthType;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth()
	{
		return this->MaxHealth;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHealth()
	{
		return this->CurrentHealth;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EShieldType GetShieldType()
	{
		return this->ShieldType;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxShield()
	{
		return this->MaxShield;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentShield()
	{
		return this->CurrentShield;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EArmorType GetArmorType()
	{
		return this->ArmorType;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetArmor()
	{
		return this->Armor;
	}

	UFUNCTION(BlueprintCallable)
	AWeaponBase *GetWeapon(EWeaponType WeaponType);

	UFUNCTION(BlueprintCallable)
	AWeaponBase *GetCurrentWeapon();

	float MyTakeDamage(float Damage, EDamageType DamageType);

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

	float DamageReduction;
	float Affinity;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AWeaponBase *EquippedWeapon;

	float NoDamageTakenDuration;
	float ShieldRegenerationPerSecond;
};
