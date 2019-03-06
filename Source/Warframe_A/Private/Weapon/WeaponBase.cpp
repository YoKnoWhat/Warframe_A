// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponBase.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/WarframeGameInstance.h"
#include "Weapon/RoundBase.h"

#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"


// Sets default values
AWeaponBase::AWeaponBase() :
	Zoom(0.5f),
	AmmoMaximum(500),
	MagazineCapacity(50),
	AmmoLeft(500),
	MagazineLeft(50)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::SetOwningCharacter(AWarframeCharacter *NewOwningCharacter)
{
	OwningCharacter = NewOwningCharacter;
}

void AWeaponBase::InitBP(int32 WeaponID, int32 Level/*Polarities, Mods*/)
{
	this->Init(static_cast<EWeaponID>(WeaponID), Level);
}

void AWeaponBase::Init(EWeaponID WeaponID, uint32 Level)
{
	UWarframeGameInstance *GameInstance = Cast<UWarframeGameInstance>(this->GetGameInstance());

	const FWeaponInfo *WeaponInfo = GameInstance->GetWeaponInfo(WeaponID);

	this->Name = WeaponInfo->Name;
	this->AmmoType = WeaponInfo->AmmoType;
	this->MagazineCapacity = WeaponInfo->Magazine;
	this->AmmoMaximum = WeaponInfo->Ammo;

	this->MagazineLeft = this->MagazineCapacity;
	this->AmmoLeft = this->AmmoMaximum;
	this->MultishotChance = 1.0f;
	this->Zoom = 1.0f;
	this->CurrentFireMode = 0;
	this->FireModeArray.SetNum(WeaponInfo->ModeInfoArray.Num());

	for (int32 i = 0; i < WeaponInfo->ModeInfoArray.Num(); ++i)
	{
		FWeaponFireMode &FireMode = this->FireModeArray[i];
		const FWeaponModeInfo &ModeInfo = WeaponInfo->ModeInfoArray[i];

		FireMode.Accuracy = ModeInfo.Accuracy;
		FireMode.ChargeRate = ModeInfo.ChargeRate;
		FireMode.PelletCount = ModeInfo.Pellets;
		FireMode.CriticalChance = ModeInfo.CriticalChance;
		FireMode.CriticalMultiplier = ModeInfo.CriticalMultiplier;
		FireMode.FalloffStart = static_cast<float>(ModeInfo.FalloffStart);
		FireMode.FalloffEnd = static_cast<float>(ModeInfo.FalloffEnd);
		FireMode.FalloffDamage = ModeInfo.FalloffDamage;
		FireMode.FireRate = ModeInfo.FireRate;
		FireMode.NoiseType = ModeInfo.Noise;
		FireMode.PunchThrough = static_cast<float>(ModeInfo.PunchThrough);
		FireMode.ReloadTime = ModeInfo.Reload;
		FireMode.StatusChance = ModeInfo.Status;
		FireMode.TriggerType = ModeInfo.Trigger;

		FireMode.BaseDamage = 0.0f;
		FireMode.TotalProportionalDamage = 0.0f;

		if (ModeInfo.Slash != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Slash, ModeInfo.Slash });
			FireMode.BaseDamage += ModeInfo.Slash;
			FireMode.TotalProportionalDamage += ModeInfo.Slash * 4.0f;
		}
		if (ModeInfo.Impact != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Impact, ModeInfo.Impact });
			FireMode.BaseDamage += ModeInfo.Impact;
			FireMode.TotalProportionalDamage += ModeInfo.Impact * 4.0f;
		}
		if (ModeInfo.Puncture != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Puncture, ModeInfo.Puncture });
			FireMode.BaseDamage += ModeInfo.Puncture;
			FireMode.TotalProportionalDamage += ModeInfo.Puncture * 4.0f;
		}
		if (ModeInfo.Heat != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Heat, ModeInfo.Heat });
			FireMode.BaseDamage += ModeInfo.Heat;
			FireMode.TotalProportionalDamage += ModeInfo.Heat;
		}
		if (ModeInfo.Cold != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Cold, ModeInfo.Cold });
			FireMode.BaseDamage += ModeInfo.Cold;
			FireMode.TotalProportionalDamage += ModeInfo.Cold;
		}
		if (ModeInfo.Electricity != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Electricity, ModeInfo.Electricity });
			FireMode.BaseDamage += ModeInfo.Electricity;
			FireMode.TotalProportionalDamage += ModeInfo.Electricity;
		}
		if (ModeInfo.Toxin != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Toxin, ModeInfo.Toxin });
			FireMode.BaseDamage += ModeInfo.Toxin;
			FireMode.TotalProportionalDamage += ModeInfo.Toxin;
		}
		if (ModeInfo.Blast != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Blast, ModeInfo.Blast });
			FireMode.BaseDamage += ModeInfo.Blast;
			FireMode.TotalProportionalDamage += ModeInfo.Blast;
		}
		if (ModeInfo.Radiation != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Radiation, ModeInfo.Radiation });
			FireMode.BaseDamage += ModeInfo.Radiation;
			FireMode.TotalProportionalDamage += ModeInfo.Radiation;
		}
		if (ModeInfo.Gas != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Gas, ModeInfo.Gas });
			FireMode.BaseDamage += ModeInfo.Gas;
			FireMode.TotalProportionalDamage += ModeInfo.Slash;
		}
		if (ModeInfo.Magnetic != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Magnetic, ModeInfo.Magnetic });
			FireMode.BaseDamage += ModeInfo.Magnetic;
			FireMode.TotalProportionalDamage += ModeInfo.Magnetic;
		}
		if (ModeInfo.Viral != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Viral, ModeInfo.Viral });
			FireMode.BaseDamage += ModeInfo.Viral;
			FireMode.TotalProportionalDamage += ModeInfo.Viral;
		}
		if (ModeInfo.Corrosive != 0.0f)
		{
			FireMode.DamageArray.Add({ EDamageType::Corrosive, ModeInfo.Corrosive });
			FireMode.BaseDamage += ModeInfo.Corrosive;
			FireMode.TotalProportionalDamage += ModeInfo.Corrosive;
		}

		// Apply mods (Specific note for base damage mod like Serration).
		this->BleedMultiplier = 1.0f;
		this->HeatModMultiplier = 1.0f;
		this->ColdModMultiplier = 1.0f;
		this->ElectricityModMultiplier = 1.0f;
		this->ToxinModMultiplier = 1.0f;

		FireMode.CriticalTier = static_cast<uint32>(FMath::TruncToInt(FireMode.CriticalChance));
		FireMode.CriticalChance -= static_cast<float>(FireMode.CriticalTier);

		FireMode.StatusChanceIntervalArray.SetNum(FireMode.DamageArray.Num());
		float InvTotalProportionalDamage = 1.0f / FireMode.TotalProportionalDamage;
		float StatusChanceInterval = 0.0f;
		for (int32 j = 0; j < FireMode.DamageArray.Num(); ++j)
		{
			auto &Pair = FireMode.DamageArray[j];

			if (EDamageType::Slash <= Pair.Type && Pair.Type <= EDamageType::Puncture)
			{
				StatusChanceInterval += Pair.Value * 4.0f * InvTotalProportionalDamage;
			}
			else
			{
				StatusChanceInterval += Pair.Value * InvTotalProportionalDamage;
			}
			FireMode.StatusChanceIntervalArray[j] = StatusChanceInterval;
		}
	}
}

void AWeaponBase::ApplyMods()
{
	// TODO
}

EDamageType AWeaponBase::GetStatusEffect(float Roll)const
{
	for (int32 i = 0; i < FireModeArray[CurrentFireMode].StatusChanceIntervalArray.Num(); ++i)
	{
		if (Roll < FireModeArray[CurrentFireMode].StatusChanceIntervalArray[i])
		{
			return FireModeArray[CurrentFireMode].DamageArray[i].Type;
		}
	}
	return EDamageType::None;
}

void AWeaponBase::SetFireMode(uint32 FireMode)
{

}

void AWeaponBase::SetFireModeBP(int32 FireMode)
{
	this->SetFireMode(static_cast<uint32>(FireMode));
}

void AWeaponBase::BeginFire()
{
	FTimerDelegate Delegate;

	Delegate.BindUObject(this, &AWeaponBase::Fire);

	this->GetGameInstance()->GetTimerManager().SetTimer(FireTimerHandle, Delegate, 1.0f / GetFireRate(), true);

	// Do fire immediately.
	this->Fire();
}

void AWeaponBase::StopFire()
{
	this->GetGameInstance()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void AWeaponBase::BeginReload()
{
	if (AmmoLeft > 0)
	{
		bIsReloading = true;

		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &AWeaponBase::Reload);

		this->GetGameInstance()->GetTimerManager().SetTimer(ReloadTimerHandle, Delegate, GetReloadTime(), false);
	}
}

void AWeaponBase::StopReload()
{
	bIsReloading = false;

	this->GetGameInstance()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

bool AWeaponBase::IsReloading()const
{
	return bIsReloading;
}

ARoundBase *AWeaponBase::OnRoundFired_Implementation(const FHitResult &CurrentTarget)
{
	return nullptr;
}

void AWeaponBase::Fire()
{
	bool ShouldCeaseFire;

	switch (GetTriggerType())
	{
		case EWeaponTriggerType::Auto:
			ShouldCeaseFire = this->Fire_Auto();
			break;
		case EWeaponTriggerType::SemiAuto:
			ShouldCeaseFire = this->Fire_SemiAuto();
			break;
		default:
			ShouldCeaseFire = true;
			break;
	}

	if (ShouldCeaseFire)
	{
		this->StopFire();
	}
}

bool AWeaponBase::Fire_Auto()
{
	if (MagazineLeft == 0)
	{
		return true;
	}
	else
	{
		MagazineLeft -= 1;

		this->DoFire();

		return false;
	}
}

bool AWeaponBase::Fire_SemiAuto()
{
	if (MagazineLeft != 0)
	{
		MagazineLeft -= 1;

		this->Fire();
	}

	return true;
}

bool AWeaponBase::Fire_Charge()
{
	if (MagazineLeft > 0)
	{
		
	}

	return true;
}

void AWeaponBase::DoFire()
{
	// todo: multishot on continuous weapon.

	// todo: accuracy & recoil

	uint32 TotalPellets;
	{
		float TotalPelletsFractional = this->GetPelletCount() * this->MultishotChance;

		TotalPellets = static_cast<uint32>(TotalPelletsFractional);

		float PelletFraction = FMath::Frac(TotalPelletsFractional);
		if (PelletFraction != 0.0f)
		{
			if (FMath::FRandRange(0.0f, 1.0f) < PelletFraction)
			{
				++TotalPellets;
			}
		}
	}

	for (uint32 i = 0; i < TotalPellets; ++i)
	{
		ARoundBase *NewRound = this->OnRoundFired(OwningCharacter->GetSelectedTarget());
	
		NewRound->Instigator = Cast<AWarframeCharacter>(this->GetOwner());
		NewRound->Init(this);
	}
}

void AWeaponBase::Reload()
{
	float MagazineToReload = FMath::Min(AmmoLeft, MagazineCapacity - MagazineLeft);

	MagazineLeft += MagazineToReload;

	AmmoLeft -= MagazineToReload;

	bIsReloading = false;
}
