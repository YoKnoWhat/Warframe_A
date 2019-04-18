
#pragma once

#include "Character/StateMachine/WarframeCharacterStateMachineComponent.h"
#include "Character/Warframe/StateMachine/WarframeLowerStates.h"
#include "Character/Warframe/StateMachine/WarframeUpperStates.h"
#include "Character/Warframe/StateMachine/WarframeAimStates.h"
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
	Crouching,
	Falling,
	Idle,
	Jumping,
	Sprinting,

	AimGliding,
	BulletJumping,
	DoubleJumping,
	Rolling,
	Sliding,
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
	Reload,
	SwitchWeapon,

	Roll,
};

struct FWarframeStateMachineLayer_Lower : public FWarframeCharacterStateMachineLayer_Lower
{
	FStateObject* AimGlidingState;
	FStateObject* BulletJumpingState;
	FStateObject* DoubleJumpingState;
	FStateObject* RollingState;
	FStateObject* SlidingState;
};

struct FWarframeStateMachineLayer_Aim : public FStateMachineLayer
{
	FStateObject* AimingState;
	FStateObject* IdleState;
};

UCLASS()
class WARFRAME_A_API UWarframeStateMachineComponent : public UWarframeCharacterStateMachineComponent
{
	GENERATED_BODY()

public:
	UWarframeStateMachineComponent();

	virtual void Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer)override;

	virtual void ReInit()override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)override;

public:
	FWarframeStateMachineLayer_Lower* LowerLayer;
	FWarframeStateMachineLayer_Aim* AimLayer;

	UPROPERTY(BlueprintReadWrite)
	bool IsAiming;

	// todo: lower layer for now.
	float BulletJumpingTimer;
	float DoubleJumpingTimer;
	float RollingTimer;
	float MaxCustomMovementSpeedBefore;

	// todo: upper layer for now.
	float FOVZoomTo;
	float CurveTime;
	bool Cache_IsAiming;
	bool HasDoubleJumped;
	float GlideTimer;

private:
	static FWarframeLowerState_AimGliding		LowerAimGlidingState;
	static FWarframeLowerState_BulletJumping	LowerBulletJumpingState;
	static FWarframeLowerState_Crouching		LowerCrouchingState;
	static FWarframeLowerState_DoubleJumping	LowerDoubleJumpingState;
	static FWarframeLowerState_Falling			LowerFallingState;
	static FWarframeLowerState_Idle				LowerIdleState;
	static FWarframeLowerState_Jumping			LowerJumpingState;
	static FWarframeLowerState_Rolling			LowerRollingState;
	static FWarframeLowerState_Sliding			LowerSlidingState;
	static FWarframeLowerState_Sprinting		LowerSprintingState;

	static FWarframeAimState_Aiming	AimAimingState;
	static FWarframeAimState_Idle	AimIdleState;
};
