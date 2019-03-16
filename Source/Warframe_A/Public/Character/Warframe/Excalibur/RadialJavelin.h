
#pragma once

#include "Character/AbilityObject.h"


class FRadialJavelin : public FAbilityObject
{
public:
	FRadialJavelin(AWarframe* Warframe);

	virtual void Tick(float DeltaTime)override;

	virtual void OnLevelChanged(uint32 NewLevel)override;

	virtual void PreCast()override;

	virtual void DoCast()override;

	virtual void PostCast()override;

protected:
	float Damage;
	uint32 Range;
	uint32 JavelinCount;
};
