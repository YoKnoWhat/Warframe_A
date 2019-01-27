// Fill out your copyright notice in the Description page of Project Settings.

#include "WarframeCharacter.h"
#include "WarframeGameInstance.h"
#include "CharacterWidgetComponent.h"


// Sets default values
AWarframeCharacter::AWarframeCharacter(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer),
	Level(1),
	Name("Unknown"),
	HealthType(EHealthType::Flesh),
	MaxHealth(100.0f),
	CurrentHealth(100.0f),
	ShieldType(EShieldType::Shield),
	MaxShield(100.0f),
	CurrentShield(100.0f),
	ArmorType(EArmorType::FerriteArmor),
	Armor(50.0f),
	DamageReduction(10.0f),
	Affinity(50.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterWidget = ObjectInitializer.CreateDefaultSubobject<UCharacterWidgetComponent>(this, FName("CharacterWidget"));
	CharacterWidget->SetupAttachment(this->RootComponent);
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

	if (ShieldRechargeTimer > 3.0f * ShieldRechargeDelayMultiplier)
	{
		if (CurrentShield != MaxShield)
		{
			CurrentShield = FMath::Min(CurrentShield + (15.0f + 0.05f * MaxShield) * ShieldRechargeSpeedMultiplier * DeltaTime, MaxShield);
		}
	}
	else
	{
		ShieldRechargeTimer += DeltaTime;
	}
}

// Called to bind functionality to input
void AWarframeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWarframeCharacter::InitPropertiesBP(int32 CharacterID, int32 Level)
{
	this->InitProperties(static_cast<ECharacterID>(CharacterID), static_cast<uint32>(Level));
}

void AWarframeCharacter::InitProperties(ECharacterID CharacterID, uint32 Level)
{
	this->Level = Level;

	UWarframeGameInstance *GameInstance = Cast<UWarframeGameInstance>(this->GetGameInstance());

	this->Name = GameInstance->GetCharacterName(CharacterID);

	const FCharacterProperties *CharacterProp = GameInstance->GetCharacterProp(CharacterID);
	
	if (CharacterProp != nullptr)
	{
		this->HealthType = CharacterProp->HealthType;
		this->MaxHealth = this->CurrentHealth = this->PropertyLevelScaling(CharacterProp->Health, CharacterProp->BaseLevel, 2.0f, 0.015f, Level);

		this->ShieldType = CharacterProp->ShieldType;
		this->MaxShield = this->CurrentShield = this->PropertyLevelScaling(CharacterProp->Shield, CharacterProp->BaseLevel, 2.0f, 0.0075f, Level);

		this->ArmorType = CharacterProp->ArmorType;
		this->Armor = this->PropertyLevelScaling(CharacterProp->Armor, CharacterProp->BaseLevel, 1.75f, 0.005f, Level);

		this->DamageReduction = CharacterProp->DamageReduction;
		this->Affinity = this->PropertyLevelScaling(CharacterProp->Affinity, CharacterProp->BaseLevel, 0.5f, 0.1425f, Level);
	}
}

AWeaponBase *AWarframeCharacter::GetWeapon(EWeaponType WeaponType)
{
	return EquippedWeapon;
}

AWeaponBase *AWarframeCharacter::GetCurrentWeapon()
{
	return EquippedWeapon;
}

float AWarframeCharacter::ApplyDamage(float Damage, EDamageType DamageType)
{
	CurrentShield -= Damage;

	if (CurrentShield < 0.0f)
	{
		CurrentHealth += CurrentShield;
		CurrentShield = 0.0f;
	}

	ShieldRechargeTimer = 0.0f;

	return Damage;
}

float AWarframeCharacter::PropertyLevelScaling(float BaseValue, float BaseLevel, float Exponent, float Coefficient, float CurrentLevel)
{
	return BaseValue * (1.0f + FMath::Pow(CurrentLevel - BaseLevel, Exponent) * Coefficient);
}
