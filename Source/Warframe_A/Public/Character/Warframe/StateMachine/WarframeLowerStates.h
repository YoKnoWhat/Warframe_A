
#pragma once

#include "Character/StateObject.h"


class WARFRAME_A_API FWarframeLowerState_AimGliding : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeLowerState_BulletJumping : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeLowerState_Crouching : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeLowerState_DoubleJumping : public FStateObject
{

public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeLowerState_Falling : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeLowerState_Idle : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeLowerState_Jumping : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeLowerState_Rolling : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeLowerState_Sliding : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeLowerState_Sprinting : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};