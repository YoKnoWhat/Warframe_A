// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"


UCLASS(BlueprintType, Blueprintable)
class WARFRAME_A_API UStateObject : public UObject
{
	GENERATED_BODY()

public:
	UStateObject()
	{
		ID = -1;
	}

	UFUNCTION(BlueprintCallable)
	void Init(int32 ID_)
	{
		ID = ID_;
	}

	// virtual FName OnUpdate_Native(AActor *Actor)
	// {
	// 	return this->OnUpdate(Actor);
	// }
	UFUNCTION(BlueprintNativeEvent)
	int32 OnUpdate(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnEnter(AActor *Actor, int32 StateFromID);

	UFUNCTION(BlueprintNativeEvent)
	void OnExit();

	UFUNCTION(BlueprintNativeEvent)
	int32 OnCustomEvent(int32 EventID);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 ID;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARFRAME_A_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// // Sets default values for this component's properties
	// UStateMachineComponent();

	UStateMachineComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void AddState(UStateObject *StateObject);

	/**
	 * Set new state with validity check.
	 */
	UFUNCTION(BlueprintCallable)
	bool SetState(int32 StateID);

	// void SetState(UStateObject *NewState);

	UFUNCTION(BlueprintCallable)
	UStateObject *GetState(int32 StateID);

	UFUNCTION(BlueprintCallable)
	UStateObject *GetCurrentState();

	UFUNCTION(BlueprintCallable)
	void TriggerEvent(int32 EventID);
	
protected:
	UStateObject *CurrentState;

	UPROPERTY()
	TMap<int32, UStateObject*> CachedStates;
};
