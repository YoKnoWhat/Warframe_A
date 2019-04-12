// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponBase.h"
#include "Character/TargetSelectionComponent.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/WarframeGameInstance.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Weapon/RoundBase.h"
#include "Weapon/TriggerModifier.h"

#include "Runtime/AIModule/Classes/Perception/AISense_Hearing.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/SkeletalMesh.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/Engine/Public/TimerManager.h"


// Sets default values
AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = Cast<UMeshComponent>(ObjectInitializer.CreateDefaultSubobject(this, "Mesh", USkeletalMeshComponent::StaticClass(), USkeletalMeshComponent::StaticClass(), true, false, false));
	MeshComponent->SetupAttachment(this->RootComponent);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastFire += DeltaTime;

	if (bIsFiring)
	{
		while (TimeSinceLastFire > GetFireInterval())
		{
			TimeSinceLastFire -= GetFireInterval();

			if (FireModeArray[CurrentFireMode].TriggerModifier->SubsequentFire())
			{
				this->StopFire();

				break;
			}
		}
	}
	else if (bIsReloading)
	{
		TimeSinceReloadBegin += DeltaTime;

		if (TimeSinceReloadBegin > GetReloadTime())
		{
			this->Reload();

			bIsReloading = false;
		}
	}
}

void AWeaponBase::Init(EWeaponID WeaponID)
{
	UWarframeGameInstance *GameInstance = Cast<UWarframeGameInstance>(this->GetGameInstance());

	const FWeaponInfo *WeaponInfo = GameInstance->GetWeaponInfo(WeaponID);

	this->SetLevel(1);

	this->Name = WeaponInfo->Name;
	this->AmmoType = WeaponInfo->AmmoType;
	this->MagazineCapacity = WeaponInfo->Magazine;
	this->AmmoMaximum = WeaponInfo->Ammo;

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
		FireMode.FireInterval = 1.0f / ModeInfo.FireRate;
		FireMode.NoiseType = ModeInfo.Noise;
		FireMode.PunchThrough = static_cast<float>(ModeInfo.PunchThrough);
		FireMode.ReloadTime = ModeInfo.Reload;
		FireMode.StatusChance = ModeInfo.Status;
		FireMode.TriggerType = ModeInfo.Trigger;
		FireMode.TriggerModifier = this->InitTriggerModifier(ModeInfo);

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

		// todo: Apply mods (Specific note for base damage mod like Serration).

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

	this->BleedMultiplier = 1.0f;
	this->HeatModMultiplier = 1.0f;
	this->ColdModMultiplier = 1.0f;
	this->ElectricityModMultiplier = 1.0f;
	this->ToxinModMultiplier = 1.0f;
	this->MultishotChance = 1.0f;
	this->Zoom = 0.75f;

	this->MagazineLeft = this->MagazineCapacity;
	this->AmmoLeft = this->AmmoMaximum;
	this->CurrentFireMode = 0;

	TimeSinceLastFire = GetFireInterval() + 1.0f;

	// Set weapon appearance.
	const FWeaponAppearance *WeaponAppearance = GameInstance->GetWeaponAppearance(WeaponID);

	Cast<USkeletalMeshComponent>(this->MeshComponent)->SetSkeletalMesh(FWarframeConfigSingleton::Instance().FindResource<USkeletalMesh>(WeaponAppearance->Mesh));
	this->FireEmitter = FWarframeConfigSingleton::Instance().FindResource<UParticleSystem>(WeaponAppearance->FireEmitter);
	WeaponAppearance->ReloadAnim;
}

void AWeaponBase::SetLevel(uint32 InLevel)
{
	this->Level = InLevel;
}

UClass* AWeaponBase::GetRoundClass_Implementation()const
{
	return AWeaponBase::StaticClass();
}

ARoundBase* AWeaponBase::OnRoundFired_Implementation(const FHitResult& CurrentTarget)
{
	FVector SocketLocation = MeshComponent->GetSocketLocation("Socket_Muzzle");

	// SpawnEmitter.
	UGameplayStatics::SpawnEmitterAtLocation(this, FireEmitter, SocketLocation);
	
	FTransform SpawnTransform(
		(CurrentTarget.ImpactPoint - SocketLocation).ToOrientationRotator(),
		SocketLocation,
		FVector(1.0f, 1.0f, 1.0f)
	);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Cast<AWarframeCharacter>(this->GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ARoundBase* NewRound = Cast<ARoundBase>(this->GetWorld()->SpawnActor(GetRoundClass(), &SpawnTransform, SpawnParams));

	return NewRound;
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
	bIsFiring = true;

	if (TimeSinceLastFire > GetFireInterval())
	{
		TimeSinceLastFire = 0.0f;

		if (FireModeArray[CurrentFireMode].TriggerModifier->FirstFire())
		{
			this->StopFire();
		}
	}
}

void AWeaponBase::StopFire()
{
	bIsFiring = false;

	if (FireModeArray[CurrentFireMode].TriggerType == EWeaponTriggerType::Charge)
	{
		dynamic_cast<FTriggerModifier_Charge*>(FireModeArray[CurrentFireMode].TriggerModifier)->StopFire();
	}
}

void AWeaponBase::FireRound(float DamageScalar)
{
	if (GetNoiseType() == ENoiseType::Alarming)
	{
		UAISense_Hearing::ReportNoiseEvent(this, Instigator->GetActorLocation(), 1.0f, Instigator);
	}

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

	AController* Controller = Instigator->GetController();
	if (Controller != nullptr)
	{
		UTargetSelectionComponent* TargetSelectionComponent = Cast<UTargetSelectionComponent>(Controller->GetComponentByClass(UTargetSelectionComponent::StaticClass()));
		const FHitResult& SelectedTarget = TargetSelectionComponent->GetSelectedTarget();

		for (uint32 i = 0; i < TotalPellets; ++i)
		{
			ARoundBase *NewRound = this->OnRoundFired(SelectedTarget);
			NewRound->Init(this);
			NewRound->ApplyDamageScalar(DamageScalar);
		}
	}
}

void AWeaponBase::BeginReload()
{
	if (AmmoLeft > 0)
	{
		bIsReloading = true;

		TimeSinceReloadBegin = 0.0f;
	}
}

void AWeaponBase::StopReload()
{
	bIsReloading = false;
}

bool AWeaponBase::IsReloading()const
{
	return bIsReloading;
}

void AWeaponBase::GainMagazine(uint32 Value)
{
	MagazineLeft += Value;

	if (MagazineLeft > MagazineCapacity)
	{
		MagazineLeft = MagazineCapacity;
	}
}

void AWeaponBase::ConsumeMagazine(uint32 Value)
{
	if (MagazineLeft < Value)
	{
		MagazineLeft = 0;
	}
	else
	{
		MagazineLeft -= Value;
	}
}

void AWeaponBase::GainAmmo(uint32 Value)
{
	AmmoLeft += Value;

	if (AmmoLeft > AmmoMaximum)
	{
		AmmoLeft = AmmoMaximum;
	}
}

FTriggerModifier* AWeaponBase::InitTriggerModifier(const FWeaponModeInfo& ModeInfo)
{
	switch (ModeInfo.Trigger)
	{
	case EWeaponTriggerType::Auto:
		return new FTriggerModifier_Auto(this);
	case EWeaponTriggerType::Burst:
		return new FTriggerModifier_Burst(this, ModeInfo.MaxBurstCount);
	case EWeaponTriggerType::Charge:
		return new FTriggerModifier_Charge(this, ModeInfo.ChargeRate, ModeInfo.ChargeMultiplier, ModeInfo.MaxChargeRounds);
	case EWeaponTriggerType::SemiAuto:
		return new FTriggerModifier_SemiAuto(this);
	default:
		return new FTriggerModifier_Null();
	}
}

void AWeaponBase::Reload()
{
	float MagazineToReload = FMath::Min(AmmoLeft, MagazineCapacity - MagazineLeft);

	MagazineLeft += MagazineToReload;

	AmmoLeft -= MagazineToReload;

	bIsReloading = false;
}
