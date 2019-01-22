// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"


// Sets default values
AWeaponBase::AWeaponBase() :
	TriggerType(EWeaponTriggerType::Auto),
	FireRate(10.0f),
	Accuracy(100.0f),
	ReloadTime(1.0f),
	Zoom(0.5f),
	Damage(1.0f),
	MaxAmmo(500),
	MagazineSize(50),
	RemainingAmmo(500),
	RemainingMagazine(50)
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

void AWeaponBase::BeginFire()
{
	FTimerDelegate Delegate;

	Delegate.BindUObject(this, &AWeaponBase::DoFire);

	this->GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, Delegate, 1.0f / FireRate, true);

	// Do fire immediately.
	this->DoFire();
}

void AWeaponBase::StopFire()
{
	this->GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void AWeaponBase::BeginReload()
{
	if (RemainingAmmo > 0)
	{
		bIsReloading = true;

		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &AWeaponBase::DoReload);

		this->GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, Delegate, ReloadTime, false);
	}
}

void AWeaponBase::StopReload()
{
	bIsReloading = false;

	this->GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

bool AWeaponBase::IsReloading()const
{
	return bIsReloading;
}

void AWeaponBase::DoFire()
{
	bool ShouldCeaseFire;

	switch (TriggerType)
	{
		case EWeaponTriggerType::Auto:
			ShouldCeaseFire = this->DoFire_Auto();
			break;
		case EWeaponTriggerType::SemiAuto:
			ShouldCeaseFire = this->DoFire_SemiAuto();
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

bool AWeaponBase::DoFire_Auto()
{
	if (RemainingMagazine == 0)
	{
		return true;
	}
	else
	{
		RemainingMagazine -= 1;
		return false;
	}
}

bool AWeaponBase::DoFire_SemiAuto()
{
	if (RemainingMagazine != 0)
	{
		RemainingMagazine -= 1;
	}

	return true;
}

bool AWeaponBase::DoFire_Charge()
{
	if (RemainingMagazine > 0)
	{
		
	}

	return true;
}

void AWeaponBase::DoReload()
{
	float MagazineToReload = FMath::Min(RemainingAmmo, MagazineSize - RemainingMagazine);

	RemainingMagazine += MagazineToReload;

	RemainingAmmo -= MagazineToReload;
}
