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
	this->BaseDamagePhys = Weapon->GetBaseDamagePhys();
	this->DamageArray = Weapon->GetDamageArray();

	// Compute extra attributes.

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
		float DamageScalar = 1.0f;

		// Apply falloff scalar.
		if (FalloffEnd > 0.0f)
		{
			float FalloffScalar = (FalloffEnd - FliedDistance) / (FalloffEnd - FalloffStart);

			DamageScalar *= FMath::Clamp(FalloffScalar, this->FalloffDamage, 1.0f);
		}

		//// Apply punch through scalar.
		//if (PunchThrough > 1.0f)
		//{
		//	PunchThrough -= 1.0f;
		//}
		//else // if (PunchThrough <= 1.0f)
		//{
		//	DamageScalar *= PunchThrough;
		//	PunchThrough = 0.0f;
		//}

		// Apply cirtical hit scalar.
		DamageScalar *= (1.0 + CriticalTier * (CriticalMultiplier - 1.0f));

		AActor* DamageCauser;
		if (this->GetOwner() != nullptr)
		{
			DamageCauser = this->GetOwner()->GetOwner();
		}
		else
		{
			DamageCauser = nullptr;
		}

		WarframeCharacter->ApplyDamage(DamageCauser, HitLocation, this, DamageScalar);
	}
	else
	{
		// PunchThrough
	}
}
