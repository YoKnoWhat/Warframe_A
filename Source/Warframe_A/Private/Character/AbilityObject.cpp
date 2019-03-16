
#include "Character/AbilityObject.h"


FAbilityObject::FAbilityObject(AWarframe* Warframe_, bool CanCastInAir, bool CanRecast, float PreCastTime_, float PostCastTime_) :
	Warframe(Warframe_),
	bCanCastInAir(CanCastInAir),
	bCanRecast(CanRecast),
	PreCastTime(PreCastTime_),
	PostCastTime(PostCastTime_),
	bIsLock(true),
	bIsActive(false)
{}

void FAbilityObject::Tick(float DeltaTime)
{
	if (bIsActive)
	{
		if (CastProgress <= PreCastTime)
		{
			/**
			 *     Pre    Post
			 * |---------|----|
			 *    ^
			 */
			CastProgress += DeltaTime;

			if (CastProgress > PreCastTime)
			{
				this->DoCast();

				if (CastProgress > PostCastTime)
				{
					this->PostCast();
					bIsActive = false;
				}
			}
		}
		else
		{
			/**
			 *     Pre    Post
			 * |---------|----|
			 *             ^
			 */
			CastProgress += DeltaTime;

			if (CastProgress > PostCastTime)
			{
				this->PostCast();
				bIsActive = false;
			}
		}
	}
}

void FAbilityObject::OnLevelChanged(uint32 NewLevel)
{
	if (NewLevel == static_cast<uint32>(-1))
	{
		bIsLock = true;
	}
	else
	{
		bIsLock = false;
	}
}

void FAbilityObject::Cast(float Charge_)
{
	if (bIsLock == false && (bIsActive == false || (bIsActive && bCanRecast)))
	{
		bIsActive = true;
		Charge = Charge_;
		CastProgress = 0.0f;

		this->PreCast();
	}
}

float FAbilityObject::GetDuration()const
{
	return 0.0f;
}



FAbilityObject_Null::FAbilityObject_Null() :
	FAbilityObject(nullptr, false, false, 0.0f, 0.0f)
{}

void FAbilityObject_Null::Tick(float DeltaTime)
{}

void FAbilityObject_Null::OnLevelChanged(uint32 NewLevel)
{}

void FAbilityObject_Null::PreCast()
{}

void FAbilityObject_Null::DoCast()
{}

void FAbilityObject_Null::PostCast()
{}
