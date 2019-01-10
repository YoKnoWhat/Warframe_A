// Fill out your copyright notice in the Description page of Project Settings.

#include "StateMachineComponent.h"


UStateObject *UStateObject::OnUpdate_Implementation(AActor *Actor, float DeltaTime)
{
	return this;
}

void UStateObject::OnEnter_Implementation(AActor *Actor, int32 StateID)
{}

void UStateObject::OnExit_Implementation(AActor *Actor)
{}


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

	UStateObject *NewState = CurrentState->OnUpdate(this->GetOwner(), DeltaTime);

	this->SetState(NewState);
}

void UStateMachineComponent::AddState(UStateObject *StateObject)
{
	CachedStates.Add(StateObject->ID, StateObject);
}

void UStateMachineComponent::SetState(int32 StateID)
{
	if (StateID != CurrentState->ID)
	{
		UStateObject *NewState = this->GetState(StateID);

		if (NewState != nullptr)
		{
			CurrentState->OnExit(this->GetOwner());
			NewState->OnEnter(this->GetOwner(), CurrentState->ID);

			CurrentState = NewState;
		}
	}
}

void UStateMachineComponent::SetState(UStateObject *NewState)
{
	if (NewState != nullptr && NewState != CurrentState)
	{
		CurrentState->OnExit(this->GetOwner());
		NewState->OnEnter(this->GetOwner(), CurrentState->ID);

		CurrentState = NewState;
	}
}

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
