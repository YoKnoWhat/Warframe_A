// Fill out your copyright notice in the Description page of Project Settings.

#include "WarframeCharacter.h"
#include "WarframeGameInstance.h"
#include "CharacterWidgetComponent.h"


// Sets default values
AWarframeCharacter::AWarframeCharacter(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer)
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
		this->MaxHealth = this->CurrentHealth = CharacterProp->Health;

		this->ShieldType = CharacterProp->ShieldType;
		this->MaxShield = this->CurrentShield = CharacterProp->Shield;

		this->ArmorType = CharacterProp->ArmorType;
		this->Armor = CharacterProp->Armor;

		this->DamageReduction = CharacterProp->DamageReduction;
		this->Affinity = CharacterProp->Affinity;
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
