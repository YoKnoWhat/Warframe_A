
#pragma once

#include "Character/StateMachineComponent.h"
#include "WarframeStateMachineComponent.generated.h"


UENUM(BlueprintType)
enum class EWarframeStateLayer : uint8
{
	Lower,
	Upper,
	Aim,
	OffHand,
};

UENUM(BlueprintType)
enum class EWarframeLowerState : uint8
{
	AimGliding,
	BulletJumping,
	Crouching,
	DoubleJumping,
	Falling,
	Idle,
	Jumping,
	Rolling,
	Sliding,
	Sprinting,
};

UENUM(BlueprintType)
enum class EWarframeUpperState : uint8
{
	Idle,
	Reloading,
	Firing,
	WeaponSwitching,
	Ironsight,
};

UENUM(BlueprintType)
enum class EWarframeAimState : uint8
{
	Idle,
	Aiming,
};

UENUM(BlueprintType)
enum class EWarframeActionEvent : uint8
{
	Jump,
	Roll,
	Reload,
	SwitchWeapon,
};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WARFRAME_A_API UWarframeStateMachineComponent : public UStateMachineComponent
{
	GENERATED_BODY()

public:
	UWarframeStateMachineComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintReadWrite)
	bool IsAiming;

	UPROPERTY(BlueprintReadWrite)
	bool IsSprinting;

	UPROPERTY(BlueprintReadWrite)
	bool IsCrouching;

	UPROPERTY(BlueprintReadWrite)
	bool IsFiring;

	bool HasDoubleJumped;
	float GlideTimer;
	float TimeSinceLastFired;
};
