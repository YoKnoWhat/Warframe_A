// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "WarframeCommon.h"
#include "Character/StateMachineLayerInitializer.h"
#include "StateMachineComponent.generated.h"


struct FStateMachineLayer
{
	bool IsEnabled;

	FStateObject *CurrentState;
};

UCLASS()
class WARFRAME_A_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateMachineComponent();

	virtual ~UStateMachineComponent();

public:	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)override;

	/**
	 * Set init states.
	 */
	virtual void Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer);

	UFUNCTION(BlueprintCallable)
	virtual void ReInit();

	FORCEINLINE AWarframeCharacter* GetCharacter()const
	{
		return Character;
	}

	UFUNCTION(BlueprintCallable)
	void EnableAll();
	
	UFUNCTION(BlueprintCallable)
	void Enable(int32 LayerIndex);

	UFUNCTION(BlueprintCallable)
	void DisableAll();

	UFUNCTION(BlueprintCallable)
	void Disable(int32 LayerIndex);

	/**
	 * Set new state with validity check.
	 */
	bool SetState(int32 LayerIndex, FStateObject* NewState);

	FStateObject* GetCurrentState(int32 LayerIndex = 0);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentStateID(int32 LayerIndex = 0);

	UFUNCTION(BlueprintCallable)
	void TriggerEvent(int32 EventID);

private:
	AWarframeCharacter* Character;

	TMap<int32, FStateMachineLayer*> Layers;
};
