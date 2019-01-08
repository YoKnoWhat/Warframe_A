// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"


struct FMyState
{
	TBaseDelegate<const FMyState*, AActor*> OnUpdate;
	TBaseDelegate<void, AActor*, const FName &> OnEnter;
	TBaseDelegate<void, AActor*, const FName &> OnExit;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	
	UFUNCTION(BlueprintCallable)
	FMyState *AddState(const FName &StateName);

	UFUNCTION(BlueprintCallable)
	void SetState(const FName &StateName);

	UFUNCTION(BlueprintCallable)
	FMyState *GetState(const FName &StateName);
	
protected:
	const FMyState *CurrentState;

	TMap<FName, FMyState> CachedStates;
};
