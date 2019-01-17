// Fill out your copyright notice in the Description page of Project Settings.

#include "StateMachineComponent.h"


int32 UStateObject::OnUpdate_Implementation(float DeltaTime)
{
	return -1;
}

void UStateObject::OnEnter_Implementation(AActor *Actor, int32 StateID)
{}

void UStateObject::OnExit_Implementation()
{}

int32 UStateObject::OnCustomEvent_Implementation(int32 EventID)
{
	return -1;
}


// // Sets default values for this component's properties
// UStateMachineComponent::UStateMachineComponent()
// {
// 	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
// 	// off to improve performance if you don't need them.
// 	PrimaryComponentTick.bCanEverTick = true;
// }

UStateMachineComponent::UStateMachineComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
 	// off to improve performance if you don't need them.
 	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentState = NewObject<UStateObject>(this, "DefaultStateObject");

	this->AddState(CurrentState);
}


// Called every frame
void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	int32 NewStateID;
	bool hasStateTransited;

	do
	{
		NewStateID = CurrentState->OnUpdate(DeltaTime);

		hasStateTransited = this->SetState(NewStateID);

		DeltaTime = 0.0f;
	} while (hasStateTransited);
}

void UStateMachineComponent::AddState(UStateObject *StateObject)
{
	CachedStates.Add(StateObject->ID, StateObject);
}

bool UStateMachineComponent::SetState(int32 StateID)
{
	if (StateID >= 0 && StateID != CurrentState->ID)
	{
		UStateObject *NewState = this->GetState(StateID);

		if (NewState != nullptr)
		{
			CurrentState->OnExit();
			NewState->OnEnter(this->GetOwner(), CurrentState->ID);

			CurrentState = NewState;

			return true;
		}
	}
	return false;
}

/*void UStateMachineComponent::SetState(UStateObject *NewState)
{
	if (NewState != nullptr && NewState != CurrentState)
	{
		CurrentState->OnExit(this->GetOwner());
		NewState->OnEnter(this->GetOwner(), CurrentState->ID);

		CurrentState = NewState;
	}
}*/

UStateObject *UStateMachineComponent::GetState(int32 StateID)
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

UStateObject *UStateMachineComponent::GetCurrentState()
{
	return CurrentState;
}

void UStateMachineComponent::TriggerEvent(int32 EventID)
{
	int32 NewStateID = CurrentState->OnCustomEvent(EventID);

	this->SetState(NewStateID);
}
