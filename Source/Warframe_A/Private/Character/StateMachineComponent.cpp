// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/StateMachineComponent.h"
#include "Character/StateObject.h"


// Sets default values for this component's properties
UStateMachineComponent::UStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
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

	for (int32 LayerIndex = 0; LayerIndex < this->Layers.Num(); ++LayerIndex)
	{
		auto &Layer = this->Layers[LayerIndex];

		if (Layer.IsEnabled)
		{
			DeltaTimeCopy = DeltaTime;
			do
			{
				NewStateID = Layer.CurrentState->OnUpdate(DeltaTimeCopy);

				hasStateTransited = this->SetState(NewStateID, LayerIndex);

				DeltaTimeCopy = 0.0f;
			} while (hasStateTransited);
		}
	}
}

void UStateMachineComponent::Init(const TArray<int32> &InitStateIDs)
{
	this->ClearAllLayerObjects();

	// Instantiate state objects for each layer.
	for (int32 LayerIndex = 0; LayerIndex < this->Layers.Num(); ++LayerIndex)
	{
		auto &Layer = this->Layers[LayerIndex];

		Layer.StateObjectInstances.Reserve(Layer.StateObjectClasses.Num());
		int32 index = 0;

		for (auto &Class : Layer.StateObjectClasses)
		{
			UStateObject *NewStateObject = NewObject<UStateObject>(this, Class);

			Layer.StateObjectInstances.Add(NewStateObject->GetID(), NewStateObject);
		}

		// Set initial state.
		if (InitStateIDs[LayerIndex] == -1)
		{
			Layer.IsEnabled = false;

			Layer.CurrentState = Layer.StateObjectInstances.CreateConstIterator()->Value;

			if (Layer.CurrentState != nullptr)
			{
				Layer.CurrentState->OnEnter(this->GetOwner(), -1);
			}
		}
		else
		{
			Layer.IsEnabled = true;

			Layer.CurrentState = this->GetState(InitStateIDs[LayerIndex], LayerIndex);

			Layer.CurrentState->OnEnter(this->GetOwner(), -1);
		}
	}

	// Enable ticking.
	this->SetComponentTickEnabled(true);
}

void UStateMachineComponent::EnableAll()
{
	for (auto &Layer : this->Layers)
	{
		Layer.IsEnabled = true;
	}
}

void UStateMachineComponent::Enable(int32 LayerIndex)
{
	if (this->Layers.Num() <= LayerIndex)
	{
		return;
	}

	this->Layers[LayerIndex].IsEnabled = true;
}

void UStateMachineComponent::DisableAll()
{
	for (auto &Layer : this->Layers)
	{
		Layer.IsEnabled = false;
	}
}

void UStateMachineComponent::Disable(int32 LayerIndex)
{
	if (this->Layers.Num() <= LayerIndex)
	{
		return;
	}

	this->Layers[LayerIndex].IsEnabled = false;
}

void UStateMachineComponent::ClearAllLayerClasses()
{
	for (auto &Layer : Layers)
	{
		Layer.StateObjectClasses.Empty();
	}
}

void UStateMachineComponent::ClearAllLayerObjects()
{
	for (auto &Layer : Layers)
	{
		// Reverse unused memory for re-initialization.
		Layer.StateObjectInstances.Empty(Layer.StateObjectInstances.Num());
	}
}

void UStateMachineComponent::ClearLayerClasses(int32 LayerIndex)
{
	this->Layers[LayerIndex].StateObjectClasses.Empty();
}

void UStateMachineComponent::ClearLayerObjects(int32 LayerIndex)
{
	auto &InstanceContainer = this->Layers[LayerIndex].StateObjectInstances;

	// Reverse unused memory for re-initialization.
	InstanceContainer.Empty(InstanceContainer.Num());
}

void UStateMachineComponent::AddStateClass(TSubclassOf<UStateObject> StateObjectClass, int32 LayerIndex)
{
	if (this->Layers.Num() <= LayerIndex)
	{
		return;
	}

	this->Layers[LayerIndex].StateObjectClasses.Add(StateObjectClass);
}

bool UStateMachineComponent::SetState(int32 StateID, int32 LayerIndex)
{
	if (this->Layers.Num() <= LayerIndex)
	{
		return false;
	}

	auto &CurrentState = this->Layers[LayerIndex].CurrentState;

	if (StateID != CurrentState->GetID())
	{
		UStateObject *NewState = this->GetState(StateID, LayerIndex);

		if (NewState != nullptr)
		{
			CurrentState->OnExit();
			NewState->OnEnter(this->GetOwner(), CurrentState->GetID());

			CurrentState = NewState;

			return true;
		}
	}
	return false;
}

UStateObject *UStateMachineComponent::GetState(int32 StateID, int32 LayerIndex)
{
	if (this->Layers.Num() <= LayerIndex)
	{
		return nullptr;
	}

	UStateObject** Result = this->Layers[LayerIndex].StateObjectInstances.Find(StateID);

	if (Result == nullptr)
	{
		return nullptr;
	}
	else
	{
		return *Result;
	}
}

UStateObject *UStateMachineComponent::GetCurrentState(int32 LayerIndex)
{
	if (this->Layers.Num() <= LayerIndex)
	{
		return nullptr;
	}

	return this->Layers[LayerIndex].CurrentState;
}

void UStateMachineComponent::TriggerEvent(int32 EventID)
{
	for (int32 LayerIndex = 0; LayerIndex < this->Layers.Num(); ++LayerIndex)
	{
		auto &Layer = this->Layers[LayerIndex];

		if (Layer.IsEnabled)
		{
			int32 NewStateID = Layer.CurrentState->OnCustomEvent(EventID);

			this->SetState(NewStateID, LayerIndex);
		}
	}
}
