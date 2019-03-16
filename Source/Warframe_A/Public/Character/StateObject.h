#pragma once

#include "WarframeCommon.h"


class FStateObject
{
public:
	void Init(AWarframeCharacter* Character);

	virtual int32 GetID()const = 0;

	virtual int32 OnUpdate(float DeltaTime) = 0;

	virtual void OnEnter(int32 StateFromID) = 0;

	virtual void OnExit() = 0;

	virtual int32 OnCustomEvent(int32 EventID) = 0;

protected:
	AWarframeCharacter* Character;
};
