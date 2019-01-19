#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateObject.generated.h"

/**
 * UStateObject
 */
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
	virtual int32 OnUpdate_Implementation(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnEnter(UObject *Owner, int32 StateFromID);
	virtual void OnEnter_Implementation(UObject *Owner, int32 StateFromID);

	UFUNCTION(BlueprintNativeEvent)
	void OnExit();
	virtual void OnExit_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	int32 OnCustomEvent(int32 EventID);
	virtual int32 OnCustomEvent_Implementation(int32 EventID);

	void TickSubFSMs(float DeltaTime);

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 ID;

	TArray<TBaseDelegate<bool, float>> SubFSMTickDelegates;
};
