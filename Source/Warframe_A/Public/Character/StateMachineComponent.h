// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarframeCommon.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"


struct FStateMachineLayer
{
	bool IsEnabled;

	FStateObject *CurrentState;

	TMap<int32, FStateObject*> StateObjects;
};

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARFRAME_A_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachineComponent();

	virtual ~UStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Clear state object instances in all layers and instantiate state objects from assigned state object classes in layers.
	 * @param InitStateIDs - Initial state IDs for each layer. One layer can be initially disabled by specify -1, first state in the layer will be the default state then.
	 */
	UFUNCTION(BlueprintCallable)
	void Init(const TMap<int32, int32> &InitStateIDs);

	UFUNCTION(BlueprintCallable)
	void EnableAll();

	/**
	 * Enable component state machine ticking.
	 */
	UFUNCTION(BlueprintCallable)
	void Enable(int32 LayerIndex);

	UFUNCTION(BlueprintCallable)
	void DisableAll();

	/**
	 * Disable component state machine ticking.
	 */
	UFUNCTION(BlueprintCallable)
	void Disable(int32 LayerIndex);

	/**
	 * Clear state object instances in all layers.
	 */
	void ClearAllLayers();

	/**
	 * Clear state object instances in specified layer.
	 */
	void ClearLayer(int32 LayerIndex);

	void AddStateObject(int32 LayerIndex, FStateObject* StateObject);

	/**
	 * Set new state with validity check.
	 */
	UFUNCTION(BlueprintCallable)
	bool SetState(int32 LayerIndex, int32 StateID);

	// void SetState(UStateObject *NewState);

	FStateObject* GetState(int32 LayerIndex, int32 StateID);

	FStateObject* GetCurrentState(int32 LayerIndex = 0);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentStateID(int32 LayerIndex = 0);

	UFUNCTION(BlueprintCallable)
	void TriggerEvent(int32 EventID);

protected:
	TMap<int32, FStateMachineLayer> Layers;
};
