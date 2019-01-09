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
		Name = FName("None");
	}

	UFUNCTION(BlueprintCallable)
	void Init(const FName &Name_)
	{
		Name = Name_;
	}

	// virtual FName OnUpdate_Native(AActor *Actor)
	// {
	// 	return this->OnUpdate(Actor);
	// }
	UFUNCTION(BlueprintNativeEvent)
	FName OnUpdate(AActor *Actor);

	UFUNCTION(BlueprintNativeEvent)
	void OnEnter(AActor *Actor, const FName &StateFromName);

	UFUNCTION(BlueprintNativeEvent)
	void OnExit(AActor *Actor);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FName Name;
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

	UFUNCTION(BlueprintCallable)
	void SetState(const FName &StateName);

	UFUNCTION(BlueprintCallable)
	UStateObject *GetState(const FName &StateName);
	
protected:
	UStateObject *CurrentState;

	UPROPERTY()
	TMap<FName, UStateObject*> CachedStates;
};
