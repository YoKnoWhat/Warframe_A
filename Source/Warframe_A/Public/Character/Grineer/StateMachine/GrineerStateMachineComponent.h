
#pragma once

#include "Character/StateMachineComponent.h"
#include "GrineerStateMachineComponent.generated.h"


UENUM(BlueprintType)
enum class EGrineerStateLayer : uint8
{
	Lower,
	Upper,
};

UENUM(BlueprintType)
enum class EGrineerLowerState : uint8
{
	Crouching,
	Falling,
	Idle,
	Jumping,
	Sprinting,
};

UENUM(BlueprintType)
enum class EGrineerUpperState : uint8
{
	Idle,
	Reloading,
	Firing,
	Ironsight,
};

UENUM(BlueprintType)
enum class EGrineerActionEvent : uint8
{
	Jump,
	Reload,
};

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class WARFRAME_A_API UGrineerStateMachineComponent : public UStateMachineComponent
{
	GENERATED_BODY()

public:
	UGrineerStateMachineComponent();

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
