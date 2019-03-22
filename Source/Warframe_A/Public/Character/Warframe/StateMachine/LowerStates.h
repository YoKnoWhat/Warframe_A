
#pragma once

#include "Character/StateObject.h"


class WARFRAME_A_API FLowerState_AimGliding : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};

class WARFRAME_A_API FLowerState_BulletJumping : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;

protected:
	float Time;
};

class WARFRAME_A_API FLowerState_Crouching : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};

class WARFRAME_A_API FLowerState_DoubleJumping : public FStateObject
{

public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;

protected:
	float Time;
};

class WARFRAME_A_API FLowerState_Falling : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};

class WARFRAME_A_API FLowerState_Idle : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;
};

class WARFRAME_A_API FLowerState_Jumping : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;

protected:
	float Time;
};

class WARFRAME_A_API FLowerState_Rolling : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;

protected:
	float Time;
};

class WARFRAME_A_API FLowerState_Sliding : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;

protected:
	float MaxCustomMovementSpeedBefore;
};

class WARFRAME_A_API FLowerState_Sprinting : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual int32 OnUpdate(float DeltaTime)override;

	virtual void OnEnter(int32 StateFromID)override;

	virtual void OnExit()override;

	virtual int32 OnCustomEvent(int32 EventID)override;

protected:
	float MaxWalkSpeedBerore;
};