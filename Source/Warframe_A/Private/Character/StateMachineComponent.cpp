// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/StateMachineComponent.h"
#include "Character/StateObject.h"
#include "Character/WarframeCharacter.h"


UStateMachineComponent::UStateMachineComponent() :
	Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

UStateMachineComponent::~UStateMachineComponent()
{
	for (auto& Pair : this->Layers)
	{
		delete Pair.Value;
	}
}

// Called every frame
void UStateMachineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FStateObject* NewState;
	bool hasStateTransited;
	float DeltaTimeCopy;

	for (auto& Pair : this->Layers)
	{
		if (Pair.Value->IsEnabled)
		{
			DeltaTimeCopy = DeltaTime;
			do
			{
				NewState = Pair.Value->CurrentState->OnUpdate(this, DeltaTimeCopy);

				hasStateTransited = this->SetState(Pair.Key, NewState);

				DeltaTimeCopy = 0.0f;
			} while (hasStateTransited);
		}
	}
}

void UStateMachineComponent::Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer)
{
	this->Character = InCharacter;

	for (auto& Pair : LayerInitializer.LayerMap)
	{
		this->Layers.Add(Pair.Key, Pair.Value);
	}
}

void UStateMachineComponent::ReInit()
{
	this->SetComponentTickEnabled(true);
}

void UStateMachineComponent::EnableAll()
{
	for (auto& Pair : this->Layers)
	{
		Pair.Value->IsEnabled = true;
	}

	this->SetComponentTickEnabled(true);
}

void UStateMachineComponent::Enable(int32 LayerIndex)
{
	FStateMachineLayer** Layer = this->Layers.Find(LayerIndex);
	if (Layer != nullptr)
	{
		(*Layer)->IsEnabled = true;
	}
}

void UStateMachineComponent::DisableAll()
{
	for (auto& Pair : this->Layers)
	{
		Pair.Value->IsEnabled = false;
	}

	this->SetComponentTickEnabled(false);
}

void UStateMachineComponent::Disable(int32 LayerIndex)
{
	FStateMachineLayer** Layer = this->Layers.Find(LayerIndex);
	if (Layer != nullptr)
	{
		(*Layer)->IsEnabled = false;
	}
}

bool UStateMachineComponent::SetState(int32 LayerIndex, FStateObject* NewState)
{
	FStateMachineLayer** Layer = this->Layers.Find(LayerIndex);
	if (Layer == nullptr)
	{
		return false;
	}

	FStateObject*& OldState = (*Layer)->CurrentState;
	if (NewState != OldState)
	{
		OldState->OnExit(this, NewState);
		NewState->OnEnter(this, OldState);

		OldState = NewState;

		return true;
	}
	return false;
}

FStateObject* UStateMachineComponent::GetCurrentState(int32 LayerIndex)
{
	FStateMachineLayer** Layer = this->Layers.Find(LayerIndex);
	if (Layer != nullptr)
	{
		return (*Layer)->CurrentState;
	}
	else
	{
		return nullptr;
	}
}

int32 UStateMachineComponent::GetCurrentStateID(int32 LayerIndex)
{
	FStateObject* CurrentState = GetCurrentState(LayerIndex);
	if (CurrentState == nullptr)
	{
		return -1;
	}
	else
	{
		return CurrentState->GetID();
	}
}

void UStateMachineComponent::TriggerEvent(int32 EventID)
{
	for (auto& Pair : this->Layers)
	{
		if (Pair.Value->IsEnabled)
		{
			FStateObject* NewState = Pair.Value->CurrentState->OnCustomEvent(this, EventID);

			this->SetState(Pair.Key, NewState);
		}
	}
}
