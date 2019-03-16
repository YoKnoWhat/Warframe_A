// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/StateMachineComponent.h"
#include "Character/WarframeCharacter.h"
#include "Character/StateObject.h"


// Sets default values for this component's properties
UStateMachineComponent::UStateMachineComponent() :
	Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

UStateMachineComponent::~UStateMachineComponent()
{
	// Ensure we clear all layer objects so they can be collected back to the pool.
	this->ClearAllLayers();
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

	int32 NewStateID;
	bool hasStateTransited;
	float DeltaTimeCopy;

	for (auto& Pair : this->Layers)
	{
		if (Pair.Value.IsEnabled)
		{
			DeltaTimeCopy = DeltaTime;
			do
			{
				NewStateID = Pair.Value.CurrentState->OnUpdate(DeltaTimeCopy);

				hasStateTransited = this->SetState(Pair.Key, NewStateID);

				DeltaTimeCopy = 0.0f;
			} while (hasStateTransited);
		}
	}
}

void UStateMachineComponent::Init(const TMap<int32, int32> &InitStateIDs)
{
	for (auto& LayerPair : this->Layers)
	{
		for (auto& StatePair : LayerPair.Value.StateObjects)
		{
			StatePair.Value->Init(Cast<AWarframeCharacter>(this->GetOwner()));
		}
	}

	for (auto& Pair : InitStateIDs)
	{
		auto &Layer = this->Layers[Pair.Key];

		// Set initial state.
		if (Pair.Value == -1)
		{
			Layer.IsEnabled = false;

			Layer.CurrentState = nullptr;
			for (auto& StatePair : Layer.StateObjects)
			{
				if (StatePair.Value != nullptr)
				{
					Layer.CurrentState = StatePair.Value;
					break;
				}
			}
			if (Layer.CurrentState != nullptr)
			{
				Layer.CurrentState->OnEnter(-1);
			}
		}
		else
		{
			Layer.IsEnabled = true;

			Layer.CurrentState = this->GetState(Pair.Key, Pair.Value);
			if (Layer.CurrentState != nullptr)
			{
				Layer.CurrentState->OnEnter(-1);
			}
		}
	}

	// Enable ticking.
	this->SetComponentTickEnabled(true);
}

void UStateMachineComponent::EnableAll()
{
	for (auto& Pair : this->Layers)
	{
		Pair.Value.IsEnabled = true;
	}
}

void UStateMachineComponent::Enable(int32 LayerIndex)
{
	FStateMachineLayer* Layer = this->Layers.Find(LayerIndex);
	if (Layer != nullptr)
	{
		Layer->IsEnabled = true;
	}
}

void UStateMachineComponent::DisableAll()
{
	for (auto& Pair : this->Layers)
	{
		Pair.Value.IsEnabled = false;
	}
}

void UStateMachineComponent::Disable(int32 LayerIndex)
{
	FStateMachineLayer* Layer = this->Layers.Find(LayerIndex);
	if (Layer != nullptr)
	{
		Layer->IsEnabled = false;
	}
}

void UStateMachineComponent::ClearAllLayers()
{
	for (auto& Pair : this->Layers)
	{
		for (auto& StatePair : Pair.Value.StateObjects)
		{
			delete StatePair.Value;
		}
		Pair.Value.StateObjects.Empty(Pair.Value.StateObjects.Num());
	}
}

void UStateMachineComponent::ClearLayer(int32 LayerIndex)
{
	FStateMachineLayer* Layer = this->Layers.Find(LayerIndex);
	if (Layer != nullptr)
	{
		for (auto& StatePair : Layer->StateObjects)
		{
			delete StatePair.Value;
		}
		Layer->StateObjects.Empty(Layer->StateObjects.Num());
	}
}

void UStateMachineComponent::AddStateObject(int32 LayerIndex, FStateObject* StateObject)
{
	FStateMachineLayer* Layer = this->Layers.Find(LayerIndex);
	if (Layer == nullptr)
	{
		this->Layers.Add(LayerIndex);
		Layer = &this->Layers[LayerIndex];
	}

	auto& StateObjects = Layer->StateObjects;

	FStateObject** ObjectPtr = StateObjects.Find(StateObject->GetID());
	if (ObjectPtr == nullptr)
	{
		StateObjects.Add(StateObject->GetID(), StateObject);
	}
	else
	{
		delete *ObjectPtr;
		*ObjectPtr = StateObject;
	}
}

bool UStateMachineComponent::SetState(int32 LayerIndex, int32 StateID)
{
	FStateMachineLayer* Layer = this->Layers.Find(LayerIndex);
	if (Layer == nullptr)
	{
		return false;
	}

	auto &CurrentState = Layer->CurrentState;

	if (StateID != CurrentState->GetID())
	{
		FStateObject *NewState = this->GetState(LayerIndex, StateID);

		if (NewState != nullptr)
		{
			CurrentState->OnExit();
			NewState->OnEnter(CurrentState->GetID());

			CurrentState = NewState;

			return true;
		}
	}
	return false;
}

FStateObject* UStateMachineComponent::GetState(int32 LayerIndex, int32 StateID)
{
	FStateMachineLayer* Layer = this->Layers.Find(LayerIndex);
	if (Layer == nullptr)
	{
		return nullptr;
	}

	FStateObject** ObjectPtr = Layer->StateObjects.Find(StateID);
	if (ObjectPtr == nullptr)
	{
		return nullptr;
	}
	else
	{
		return *ObjectPtr;
	}
}

FStateObject* UStateMachineComponent::GetCurrentState(int32 LayerIndex)
{
	FStateMachineLayer* Layer = this->Layers.Find(LayerIndex);
	if (Layer != nullptr)
	{
		return Layer->CurrentState;
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
		if (Pair.Value.IsEnabled)
		{
			int32 NewStateID = Pair.Value.CurrentState->OnCustomEvent(EventID);

			this->SetState(Pair.Key, NewStateID);
		}
	}
}
