
#pragma once

#include "Character/StateObject.h"


class WARFRAME_A_API FWarframeAimState_Aiming : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;

protected:
	float MaxWalkSpeedBefore;

	float FOVZoomTo;
	float CurveTime;
	bool Cache_IsAiming;
};

class WARFRAME_A_API FWarframeAimState_Idle : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};
