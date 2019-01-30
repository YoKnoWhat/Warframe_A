// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"


class UStateObject;

USTRUCT()
struct FStateMachineLayer
{
	GENERATED_BODY()

	bool IsEnabled;

	UStateObject *CurrentState;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UStateObject>> StateObjectClasses;

	UPROPERTY()
	TMap<int32, UStateObject*> StateObjectInstances;
};

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARFRAME_A_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachineComponent();

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
	void Init(const TArray<int32> &InitStateIDs);

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
	 * Clear state object classes and instances in all layers.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearAllLayerClasses();

	/**
	 * Clear state object instances in all layers.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearAllLayerObjects();

	/**
	 * Clear state object classes in specified layer.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearLayerClasses(int32 LayerIndex);

	/**
	 * Clear state object instances in specified layer.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearLayerObjects(int32 LayerIndex);

	UFUNCTION(BlueprintCallable)
	void AddStateClass(TSubclassOf<UStateObject> StateObjectClass, int32 LayerIndex = 0);

	/**
	 * Set new state with validity check.
	 */
	UFUNCTION(BlueprintCallable)
	bool SetState(int32 StateID, int32 LayerIndex = 0);

	// void SetState(UStateObject *NewState);

	UFUNCTION(BlueprintCallable)
	UStateObject *GetState(int32 StateID, int32 LayerIndex = 0);

	UFUNCTION(BlueprintCallable)
	UStateObject *GetCurrentState(int32 LayerIndex = 0);

	UFUNCTION(BlueprintCallable)
	void TriggerEvent(int32 EventID);

protected:
	UPROPERTY(EditAnywhere)
	TArray<FStateMachineLayer> Layers;
};
