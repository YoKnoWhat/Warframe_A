
#pragma once

#include "Character/StateObject.h"


class WARFRAME_A_API FWarframeAimState_Aiming : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};

class WARFRAME_A_API FWarframeAimState_Idle : public FStateObject
{
public:
	virtual int32 GetID()const override;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)override;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)override;

	virtual void OnExit(UStateMachineComponent* StateMachine)override;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)override;
};
