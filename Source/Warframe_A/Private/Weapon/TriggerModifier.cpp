
#include "Weapon/TriggerModifier.h"
#include "Weapon/WeaponBase.h"

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Public/TimerManager.h"


FTriggerModifier_Auto::FTriggerModifier_Auto(AWeaponBase* Weapon) :
	FTriggerModifier(Weapon)
{}

bool FTriggerModifier_Auto::FirstFire()
{
	return DoFire();
}

bool FTriggerModifier_Auto::SubsequentFire()
{
	return DoFire();
}

bool FTriggerModifier_Auto::DoFire()
{
	if (Weapon->GetRemainingMagazine() == 0)
	{
		return true;
	}
	else
	{
		Weapon->ConsumeMagazine(1);

		Weapon->FireRound();

		return false;
	}
}



FTriggerModifier_Burst::FTriggerModifier_Burst(AWeaponBase* Weapon, uint32 MaxBurstCount_) :
	FTriggerModifier(Weapon),
	MaxBurstCount(MaxBurstCount_)
{}

FTriggerModifier_Burst::~FTriggerModifier_Burst()
{
	if (Weapon)
	{
		if (BurstTimerHandle.IsValid())
		{
			Weapon->GetGameInstance()->GetTimerManager().ClearTimer(BurstTimerHandle);
		}
	}
}

bool FTriggerModifier_Burst::FirstFire()
{
	if (Weapon->GetRemainingMagazine() != 0 && BurstTimerHandle.IsValid() == false)
	{
		Weapon->FireRound();

		CurrentBurstCount = FMath::Min(static_cast<uint32>(Weapon->GetRemainingMagazine()), MaxBurstCount) - 1;

		if (CurrentBurstCount != 0)
		{
			FTimerDelegate Delegate;

			Delegate.BindRaw(this, &FTriggerModifier_Burst::BurstFire);

			Weapon->GetGameInstance()->GetTimerManager().SetTimer(BurstTimerHandle, Delegate, Weapon->GetFireInterval(), true);
		}
	}
	return true;
}

bool FTriggerModifier_Burst::SubsequentFire()
{
	return true;
}

void FTriggerModifier_Burst::BurstFire()
{
	Weapon->FireRound();

	if (--CurrentBurstCount == 0)
	{
		Weapon->GetGameInstance()->GetTimerManager().ClearTimer(BurstTimerHandle);
	}
}



FTriggerModifier_Charge::FTriggerModifier_Charge(AWeaponBase* Weapon, float ChargeRate_, float ChargeMultiplier_, uint32 MaxChargeRounds_) :
	FTriggerModifier(Weapon),
	ChargeRate(ChargeRate_),
	ChargeMultiplier(ChargeMultiplier_),
	MaxChargeRounds(MaxChargeRounds_),
	CurrentChargeRounds(0)
{}

bool FTriggerModifier_Charge::FirstFire()
{
	return this->DoFire();
}

bool FTriggerModifier_Charge::SubsequentFire()
{
	return this->DoFire();
}

void FTriggerModifier_Charge::StopFire()
{
	if (CurrentChargeRounds != 0)
	{
		Weapon->FireRound(static_cast<float>(CurrentChargeRounds) / static_cast<float>(MaxChargeRounds) * ChargeMultiplier);
	}

	CurrentChargeRounds = 0;
}

bool FTriggerModifier_Charge::DoFire()
{
	if (Weapon->GetRemainingMagazine() > 0)
	{
		if (CurrentChargeRounds < MaxChargeRounds)
		{
			Weapon->ConsumeMagazine(1);

			++CurrentChargeRounds;
		}
		return false;
	}
	else if (CurrentChargeRounds != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}



FTriggerModifier_SemiAuto::FTriggerModifier_SemiAuto(AWeaponBase* Weapon) :
	FTriggerModifier(Weapon)
{}

bool FTriggerModifier_SemiAuto::FirstFire()
{
	if (Weapon->GetRemainingMagazine() != 0)
	{
		Weapon->ConsumeMagazine(1);

		Weapon->FireRound();
	}

	return true;
}

bool FTriggerModifier_SemiAuto::SubsequentFire()
{
	return true;
}
