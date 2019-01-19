// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FiniteStateMachine.generated.h"

class UStateObject;

/**
 * UFiniteStateMachine
 */
UCLASS(BlueprintType, Blueprintable)
class WARFRAME_A_API UFiniteStateMachine : public UObject
{
	GENERATED_BODY()
	
public:
	~UFiniteStateMachine();

	UFUNCTION(BlueprintCallable)
	void Init(UObject *InOwner, int32 InitStateID);

	UFUNCTION(BlueprintCallable)
	void InitAsSubFSM(UObject *InOwner, UStateObject *StateObject, int32 InitStateID);

	UFUNCTION(BlueprintCallable)
	void Enable();
	
	UFUNCTION(BlueprintCallable)
	void Disable();

public:
	// Called every frame
	bool Tick(float DeltaTime);

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
	void InitInternal(UObject *InOwner, int32 InitStateID);

protected:
	bool IsEnabled;

	UObject *Owner;
	FDelegateHandle CoreTickDelegateHandle; // Used to ref core ticker delegate.

	UStateObject *ParentStateObject;
	TBaseDelegate<bool, float> SubFSMTickDelegate; // Used to ref parent state object's ticker delegate.

	UStateObject *CurrentState;

	UPROPERTY()
		TMap<int32, UStateObject*> CachedStates;

};
