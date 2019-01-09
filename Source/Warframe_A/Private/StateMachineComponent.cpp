// Fill out your copyright notice in the Description page of Project Settings.

#include "StateMachineComponent.h"


FName UStateObject::OnUpdate_Implementation(AActor *Actor)
{
	return Name;
}

void UStateObject::OnEnter_Implementation(AActor *Actor, const FName &StateFromName)
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

	FName NewStateName = CurrentState->OnUpdate(this->GetOwner());

	this->SetState(NewStateName);
}

void UStateMachineComponent::AddState(UStateObject *StateObject)
{
	CachedStates.Add(StateObject->Name, StateObject);
}

void UStateMachineComponent::SetState(const FName &StateName)
{
	if (StateName != CurrentState->Name)
	{
		UStateObject *NewState = this->GetState(StateName);

		if (NewState != nullptr)
		{
			CurrentState->OnExit(this->GetOwner());
			NewState->OnEnter(this->GetOwner(), CurrentState->Name);

			CurrentState = NewState;
		}
	}
}

UStateObject *UStateMachineComponent::GetState(const FName &StateName)
{
	UStateObject** Result = CachedStates.Find(StateName);

	if (Result == nullptr)
	{
		return nullptr;
	}
	else
	{
		return *Result;
	}
}
