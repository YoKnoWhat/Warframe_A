
#pragma once

#include "Character/StateObject.h"


class WARFRAME_A_API FWarframeCharacterLowerState_Crouching : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeCharacterLowerState_Dead : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeCharacterLowerState_Falling : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeCharacterLowerState_Idle : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeCharacterLowerState_Jumping : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeCharacterLowerState_Sprinting : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};
