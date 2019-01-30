#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StateObject.generated.h"

/**
 * UStateObject
 */
UCLASS(BlueprintType, Blueprintable)
class WARFRAME_A_API UStateObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetID()const;
	virtual int32 GetID_Implementation()const;

	// virtual FName OnUpdate_Native(AActor *Actor)
	// {
	// 	return this->OnUpdate(Actor);
	// }
	UFUNCTION(BlueprintNativeEvent)
	int32 OnUpdate(float DeltaTime);
	virtual int32 OnUpdate_Implementation(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnEnter(AActor *Owner, int32 StateFromID);
	virtual void OnEnter_Implementation(AActor *Owner, int32 StateFromID);

	UFUNCTION(BlueprintNativeEvent)
	void OnExit();
	virtual void OnExit_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	int32 OnCustomEvent(int32 EventID);
	virtual int32 OnCustomEvent_Implementation(int32 EventID);
};
