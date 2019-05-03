
#pragma once

#include "Character/StateMachineComponent.h"
#include "Character/StateMachine/WarframeCharacterLowerStates.h"
#include "Character/StateMachine/WarframeCharacterUpperStates.h"
#include "WarframeCharacterStateMachineComponent.generated.h"


UENUM(BlueprintType)
enum class EWarframeCharacterStateLayer : uint8
{
	Lower,
	Upper,
	End,
};

UENUM(BlueprintType)
enum class EWarframeCharacterLowerState : uint8
{
	Crouching,
	Dead,
	Falling,
	Idle,
	Jumping,
	Sprinting,
};

UENUM(BlueprintType)
enum class EWarframeCharacterUpperState : uint8
{
	Idle,
	Reloading,
	Firing,
	WeaponSwitching,
	Ironsight,
};

UENUM(BlueprintType)
enum class EWarframeCharacterActionEvent : uint8
{
	Jump,
	Reload,
	SwitchWeapon,
};

struct FWarframeCharacterStateMachineLayer_Lower : public FStateMachineLayer
{
	FStateObject* CrouchingState;
	FStateObject* DeadState;
	FStateObject* FallingState;
	FStateObject* IdleState;
	FStateObject* JumpingState;
	FStateObject* SprintingState;
};

struct FWarframeCharacterStateMachineLayer_Upper : public FStateMachineLayer
{
	FStateObject* FiringState;
	FStateObject* IdleState;
	FStateObject* IronsightState;
	FStateObject* ReloadingState;
	FStateObject* WeaponSwitchingState;
};

UCLASS()
class WARFRAME_A_API UWarframeCharacterStateMachineComponent : public UStateMachineComponent
{
	GENERATED_BODY()

public:
	UWarframeCharacterStateMachineComponent();

	virtual void Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer)override;

	virtual void ReInit()override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)override;

	/**
	 * Set lower layer to dead state.
	 * Set other layers to idle state.
	 * Disable all layers.
	 */
	virtual void Kill();

public:
	static FName LowerLayerName;
	static FName UpperLayerName;

	FWarframeCharacterStateMachineLayer_Lower* LowerLayer;
	FWarframeCharacterStateMachineLayer_Upper* UpperLayer;

	bool bIsSprinting;
	bool bIsCrouching;
	bool bIsFiring;

	// todo: lower layer for now.
	float JumpingTimer;
	float MaxWalkSpeedBefore;

	// todo: upper layer for now.
	float WeaponSwitchTimer;
	float TimeSinceLastFired;

private:
	static FWarframeCharacterLowerState_Crouching	LowerCrouchingState;
	static FWarframeCharacterLowerState_Dead		LowerDeadState;
	static FWarframeCharacterLowerState_Falling		LowerFallingState;
	static FWarframeCharacterLowerState_Idle		LowerIdleState;
	static FWarframeCharacterLowerState_Jumping		LowerJumpingState;
	static FWarframeCharacterLowerState_Sprinting	LowerSprintingState;

	static FWarframeCharacterUpperState_Firing			UpperFiringState;
	static FWarframeCharacterUpperState_Idle			UpperIdleState;
	static FWarframeCharacterUpperState_Ironsight		UpperIronsightState;
	static FWarframeCharacterUpperState_Reloading		UpperReloadingState;
	static FWarframeCharacterUpperState_WeaponSwitching	UpperWeaponSwitchingState;
};
