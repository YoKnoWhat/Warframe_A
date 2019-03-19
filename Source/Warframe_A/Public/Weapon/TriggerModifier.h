
#pragma once

#include "WarframeCommon.h"

#include "Runtime/Engine/Classes/Engine/EngineTypes.h"


class FTriggerModifier
{
public:
	FTriggerModifier(AWeaponBase* Weapon_) :
		Weapon(Weapon_)
	{}

	virtual ~FTriggerModifier()
	{}

	/**
	 * The reason why I declare two methods here is that there might be a situation, where
	 * the player pulls the trigger and the weapon fires a round, then during the interval of
	 * first shot and second shot, the player releases and pulls the trigger again (No FirstFire).
	 * Should the weapon fire (SubsequentFire) or not after the fire interval? If the weapon has
	 * a Auto trigger, the answer is yes. Or, the weapon may have a Semi-Auto trigger, which
	 * should not fire in that circumstance.
	 */
	virtual bool FirstFire() = 0;

	virtual bool SubsequentFire() = 0;

protected:
	AWeaponBase* Weapon;
};

class FTriggerModifier_Auto : public FTriggerModifier
{
public:
	FTriggerModifier_Auto(AWeaponBase* Weapon);

	virtual bool FirstFire()override;

	virtual bool SubsequentFire()override;

protected:
	bool DoFire();
};

class FTriggerModifier_Burst : public FTriggerModifier
{
public:
	FTriggerModifier_Burst(AWeaponBase* Weapon, uint32 MaxBurstCount);

	virtual ~FTriggerModifier_Burst()override;

	virtual bool FirstFire()override;

	virtual bool SubsequentFire()override;

protected:
	void BurstFire();

protected:
	uint32 MaxBurstCount;

	uint32 CurrentBurstCount;
	FTimerHandle BurstTimerHandle;
};

class FTriggerModifier_Charge : public FTriggerModifier
{
public:
	FTriggerModifier_Charge(AWeaponBase* Weapon, float ChargeRate, float ChargeMultiplier, uint32 MaxChargeRounds);

	virtual bool FirstFire()override;

	virtual bool SubsequentFire()override;

	void StopFire();

	FORCEINLINE uint32 GetCurrentChargeRounds()const
	{
		return CurrentChargeRounds;
	}

protected:
	bool DoFire();

protected:
	float ChargeRate;
	float ChargeMultiplier;
	uint32 MaxChargeRounds;

	uint32 CurrentChargeRounds = 0;
};

class FTriggerModifier_Null : public FTriggerModifier
{
public:
	FTriggerModifier_Null() :
		FTriggerModifier(nullptr)
	{}

	virtual bool FirstFire()override
	{
		return true;
	}

	virtual bool SubsequentFire()override
	{
		return true;
	}
};

class FTriggerModifier_SemiAuto : public FTriggerModifier
{
public:
	FTriggerModifier_SemiAuto(AWeaponBase* Weapon);

	virtual bool FirstFire()override;

	virtual bool SubsequentFire()override;
};
