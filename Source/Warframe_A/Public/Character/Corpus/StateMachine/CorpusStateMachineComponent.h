
#pragma once

#include "Character/StateMachineComponent.h"
#include "CorpusStateMachineComponent.generated.h"


UENUM(BlueprintType)
enum class ECorpusStateLayer : uint8
{
	Lower,
	Upper,
};

UENUM(BlueprintType)
enum class ECorpusLowerState : uint8
{
	Crouching,
	Falling,
	Idle,
	Jumping,
	Sprinting,
};

UENUM(BlueprintType)
enum class ECorpusUpperState : uint8
{
	Idle,
	Reloading,
	Firing,
	Ironsight,
};

UENUM(BlueprintType)
enum class ECorpusActionEvent : uint8
{
	Jump,
	Reload,
};

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class WARFRAME_A_API UCorpusStateMachineComponent : public UStateMachineComponent
{
	GENERATED_BODY()

public:
	UCorpusStateMachineComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintReadWrite)
	bool IsSprinting;

	UPROPERTY(BlueprintReadWrite)
	bool IsCrouching;

	UPROPERTY(BlueprintReadWrite)
	bool IsFiring;

	float TimeSinceLastFired;
};
