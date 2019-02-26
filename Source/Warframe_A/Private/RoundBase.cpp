// Fill out your copyright notice in the Description page of Project Settings.

#include "RoundBase.h"
#include "WarframeCharacter.h"
#include "WeaponBase.h"


// Sets default values
ARoundBase::ARoundBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARoundBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARoundBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = this->GetActorLocation();

	FliedDistance += (NewLocation - LastLocation).SizeSquared();

	LastLocation = NewLocation;
}

void ARoundBase::InitBP(AWeaponBase* Weapon)
{
	this->Init(Weapon);
}

void ARoundBase::Init(AWeaponBase* Weapon)
{
	// Init basic attributes.
	this->CriticalMultiplier = Weapon->GetCriticalMultiplier();
	this->FalloffStart = Weapon->GetFalloffStart();
	this->FalloffEnd = Weapon->GetFalloffEnd();
	this->FalloffDamage = Weapon->GetFalloffDamage();
	this->PunchThrough = Weapon->GetPunchThrough();
	this->BaseDamage = Weapon->GetBaseDamage();
	this->DamageArray = Weapon->GetDamageArray();
	this->BleedMultiplier = Weapon->GetBleedMultiplier();
	this->HeatModMultiplier = Weapon->GetHeatModMultiplier();
	this->ColdModMultiplier = Weapon->GetColdModMultiplier();
	this->ElectricityModMultiplier = Weapon->GetElectricityModMultiplier();
	this->ToxinModMultiplier = Weapon->GetToxinModMultiplier();

	// Critical hit roll.
	this->CriticalTier = Weapon->GetCriticalTier();
	if (FMath::FRandRange(0.0f, 1.0f) < Weapon->GetCriticalChance())
	{
		++this->CriticalTier;
	}

	// Status effect roll.
	if (FMath::FRandRange(0.0f, 1.0f) < Weapon->GetStatusChance())
	{
		this->StatusEffect = Weapon->GetStatusEffect(FMath::FRandRange(0.0f, 1.0f));
		switch (StatusEffect)
		{
		case EDamageType::Slash:
			this->StatusDamageMultiplier = this->BleedMultiplier;
			break;
		case EDamageType::Heat:
			this->StatusDamageMultiplier = this->HeatModMultiplier;
			break;
		case EDamageType::Electricity:
			this->StatusDamageMultiplier = this->ElectricityModMultiplier;
			break;
		case EDamageType::Toxin:
			this->StatusDamageMultiplier = this->ToxinModMultiplier;
			break;
		case EDamageType::Gas:
			this->StatusDamageMultiplier = this->ToxinModMultiplier;
			break;
		default:
			this->StatusDamageMultiplier = 1.0f;
			break;
		}
	}
	else
	{
		this->StatusEffect = EDamageType::None;
	}

	// Miscellaneous attributes.
	this->LastLocation = this->GetActorLocation();
	this->FliedDistance = 0.0f;
}

void ARoundBase::OnHit(AActor *Target, FVector HitLocation)
{
	AWarframeCharacter* WarframeCharacter = Cast<AWarframeCharacter>(Target);

	if (WarframeCharacter != nullptr)
	{
		this->DamageScalar = 1.0f;

		// Apply falloff scalar.
		if (FalloffEnd > 0.0f)
		{
			float FalloffScalar = (FalloffEnd - FliedDistance) / (FalloffEnd - FalloffStart);

			this->DamageScalar *= FMath::Clamp(FalloffScalar, this->FalloffDamage, 1.0f);
		}

		//// todo: Apply punch through scalar.
		//if (PunchThrough > 1.0f)
		//{
		//	PunchThrough -= 1.0f;
		//}
		//else // if (PunchThrough <= 1.0f)
		//{
		//	this->DamageScalar *= PunchThrough;
		//	PunchThrough = 0.0f;
		//}

		// Apply cirtical hit scalar.
		DamageScalar *= (1.0 + CriticalTier * (CriticalMultiplier - 1.0f));

		WarframeCharacter->ApplyDamage(Instigator, HitLocation, StatusEffect, DamageArray, DamageScalar, CriticalTier);

		WarframeCharacter->ApplyStatusEffect(Instigator, HitLocation, StatusEffect, BaseDamage, StatusDamageMultiplier);
	}
	else
	{
		// todo: PunchThrough
	}
}
