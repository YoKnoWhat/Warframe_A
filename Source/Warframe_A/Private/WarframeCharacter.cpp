// Fill out your copyright notice in the Description page of Project Settings.

#include "WarframeCharacter.h"
#include "Runtime/Json/Public/Serialization/JsonReader.h"


// Sets default values
AWarframeCharacter::AWarframeCharacter() :
	NoDamageTakenDuration(0.0f),
	ShieldRegenerationPerSecond(50.0f),
	Armor(100.0f),
	MaxHealth(660.0f),
	MaxShield(450.0f),
	CurrentHealth(660.0f),
	CurrentShield(450.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWarframeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarframeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NoDamageTakenDuration > 3.0f)
	{
		if (CurrentShield != MaxShield)
		{
			CurrentShield = FMath::Min(CurrentShield + ShieldRegenerationPerSecond * DeltaTime, MaxShield);
		}
	}
	else
	{
		NoDamageTakenDuration += DeltaTime;
	}
}

// Called to bind functionality to input
void AWarframeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWarframeCharacter::InitProperties(int32 CharacterID, uint32 Level)
{
}

float AWarframeCharacter::GetArmor()
{
	return Armor;
}

float AWarframeCharacter::GetMaxHealth()
{
	return MaxHealth;
}

float AWarframeCharacter::GetMaxShield()
{
	return MaxShield;
}

float AWarframeCharacter::GetCurrentHealth()
{
	return CurrentHealth;
}

float AWarframeCharacter::GetCurrentShield()
{
	return CurrentShield;
}

AWeaponBase *AWarframeCharacter::GetWeapon(EWeaponType WeaponType)
{
	return EquippedWeapon;
}

AWeaponBase *AWarframeCharacter::GetCurrentWeapon()
{
	return EquippedWeapon;
}

float AWarframeCharacter::MyTakeDamage(float Damage, EDamageType DamageType)
{
	CurrentShield -= Damage;

	if (CurrentShield < 0.0f)
	{
		CurrentHealth += CurrentShield;
		CurrentShield = 0.0f;
	}

	NoDamageTakenDuration = 0.0f;

	return Damage;
}
