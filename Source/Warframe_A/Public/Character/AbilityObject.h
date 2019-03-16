
#pragma once

#include "WarframeCommon.h"


class FAbilityObject
{
public:
	FAbilityObject(AWarframe* Warframe, bool CanCastInAir, bool CanRecast, float PreCastTime, float PostCastTime);

	virtual void Tick(float DeltaTime);

	virtual void OnLevelChanged(uint32 NewLevel);

	void Cast(float Charge);

	virtual void PreCast() = 0;

	virtual void DoCast() = 0;

	virtual void PostCast() = 0;

	/** Get ability remaining duration in second. */
	virtual float GetDuration()const;

	FORCEINLINE bool CanCastInAir()const
	{
		return bCanCastInAir;
	}

	FORCEINLINE bool CanRecast()const
	{
		return bCanRecast;
	}

	FORCEINLINE bool IsActive()const
	{
		return bIsActive;
	}

protected:
	AWarframe* Warframe;
	bool bCanCastInAir;
	bool bCanRecast;
	float PreCastTime;
	float PostCastTime;
	
	/** Mutable variables. */
	bool bIsLock;
	bool bIsActive;
	float Charge;
	float CastProgress;
};

class FAbilityObject_Null : public FAbilityObject
{
public:
	FAbilityObject_Null();

	virtual void Tick(float DeltaTime)override;

	virtual void OnLevelChanged(uint32 NewLevel)override;

	virtual void PreCast()override;

	virtual void DoCast()override;

	virtual void PostCast()override;
};