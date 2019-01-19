// Fill out your copyright notice in the Description page of Project Settings.

#include "FiniteStateMachine.h"
#include "StateObject.h"
#include "Ticker.h"


UFiniteStateMachine::~UFiniteStateMachine()
{
	if (CoreTickDelegateHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(CoreTickDelegateHandle);
	}
	else if (ParentStateObject != nullptr)
	{
		auto CompareDelegate = [=](const TBaseDelegate<bool, float>& Handle) {return Handle.GetHandle() == SubFSMTickDelegate.GetHandle(); };

		ParentStateObject->SubFSMTickDelegates.RemoveAll(CompareDelegate);
	}
}

void UFiniteStateMachine::Init(UObject *InOwner, int32 InitStateID)
{
	this->InitInternal(InOwner, InitStateID);

	CoreTickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UFiniteStateMachine::Tick));
}

void UFiniteStateMachine::InitAsSubFSM(UObject *InOwner, UStateObject *StateObject, int32 InitStateID)
{
	this->InitInternal(InOwner, InitStateID);

	SubFSMTickDelegate.BindUObject(this, &UFiniteStateMachine::Tick);

	StateObject->SubFSMTickDelegates.Add(SubFSMTickDelegate);

	ParentStateObject = StateObject;
}

void UFiniteStateMachine::Enable()
{
	IsEnabled = true;
}

void UFiniteStateMachine::Disable()
{
	IsEnabled = false;
}

bool UFiniteStateMachine::Tick(float DeltaTime)
{
	if (IsEnabled == false)
	{
		return true;
	}

	int32 NewStateID;
	bool hasStateTransited;

	do
	{
		CurrentState->TickSubFSMs(DeltaTime);

		NewStateID = CurrentState->OnUpdate(DeltaTime);

		hasStateTransited = this->SetState(NewStateID);

		DeltaTime = 0.0f;
	} while (hasStateTransited);

	return true;
}

void UFiniteStateMachine::AddState(UStateObject *StateObject)
{
	CachedStates.Add(StateObject->ID, StateObject);
}

bool UFiniteStateMachine::SetState(int32 StateID)
{
	if (StateID >= 0 && StateID != CurrentState->ID)
	{
		UStateObject *NewState = this->GetState(StateID);

		if (NewState != nullptr)
		{
			CurrentState->OnExit();
			NewState->OnEnter(Owner, CurrentState->ID);

			CurrentState = NewState;

			return true;
		}
	}
	return false;
}

/*void UFiniteStateMachine::SetState(UStateObject *NewState)
{
	if (NewState != nullptr && NewState != CurrentState)
	{
		CurrentState->OnExit(this->GetOwner());
		NewState->OnEnter(this->GetOwner(), CurrentState->ID);

		CurrentState = NewState;
	}
}*/

UStateObject *UFiniteStateMachine::GetState(int32 StateID)
{
	UStateObject** Result = CachedStates.Find(StateID);

	if (Result == nullptr)
	{
		return nullptr;
	}
	else
	{
		return *Result;
	}
}

UStateObject *UFiniteStateMachine::GetCurrentState()
{
	return CurrentState;
}

void UFiniteStateMachine::TriggerEvent(int32 EventID)
{
	int32 NewStateID = CurrentState->OnCustomEvent(EventID);

	this->SetState(NewStateID);
}

void UFiniteStateMachine::InitInternal(UObject *InOwner, int32 InitStateID)
{
	IsEnabled = true;

	Owner = InOwner;

//	CurrentState = NewObject<UStateObject>(this, "DefaultStateObject");

	CurrentState = this->GetState(InitStateID);

	CurrentState->OnEnter(Owner, -1);
}