// Fill out your copyright notice in the Description page of Project Settings.

#include "StateMachineComponent.h"


// Sets default values for this component's properties
UStateMachineComponent::UStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CachedStates.Add(FName(""), FMyState());

	CurrentState = this->GetState(FName(""));
}


// Called when the game starts
void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState->OnUpdate.IsBound())
	{
		const FMyState *NewState = CurrentState->OnUpdate.Execute(this->GetOwner());

		if (NewState != CurrentState)
		{
			CurrentState->OnExit.ExecuteIfBound();
			CurrentState = NewState;
			NewState->OnEnter.ExecuteIfBound();
		}
	}
}

FMyState *UStateMachineComponent::AddState(const FName &StateName)
{
	checkSlow(StateName != FName(""));

	return &CachedStates.Add(StateName, FMyState());
}

void UStateMachineComponent::SetState(const FName &StateName)
{
	const FMyState *NewState = this->GetState(StateName);

	if (NewState != nullptr)
	{
		CurrentState = NewState;

		CurrentState->OnEnter.ExecuteIfBound();
	}
}

FMyState *UStateMachineComponent::GetState(const FName &StateName)
{
	return CachedStates.Find(StateName);
}


const FMyState* OnUpdate_Ironsight(AActor *Actor)
{}

void MyFunc()
{
	UStateMachineComponent sm;

	
	TBaseDelegate<const FMyState*, AActor*> OnUpdate_Ironsight_Delegate;
	OnUpdate_Ironsight_Delegate.BindStatic(OnUpdate_Ironsight);

	FMyState *NewState;
	
	NewState = sm.AddState("Ironsight");
	NewState.
	sm.AddState("Sprint");
	sm.AddState("Crouch");
	sm.AddState("Slide");

}

