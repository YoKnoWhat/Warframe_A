#pragma once

#include "WarframeCommon.h"


class UStateMachineComponent;

class FStateObject
{
public:
	virtual int32 GetID()const = 0;

	virtual FStateObject* OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime) = 0;

	virtual void OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom) = 0;

	virtual void OnExit(UStateMachineComponent* StateMachine) = 0;

	virtual FStateObject* OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID) = 0;
};
