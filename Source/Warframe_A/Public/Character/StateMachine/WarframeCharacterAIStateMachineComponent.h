
#pragma once

#include "Character/StateMachine/WarframeCharacterStateMachineComponent.h"
#include "Character/StateMachine/WarframeCharacterAILowerStates.h"
#include "WarframeCharacterAIStateMachineComponent.generated.h"


UENUM(BlueprintType)
enum class EWarframeCharacterAIStateLayer : uint8
{
	Lower,
	Upper,
	End,
};

UENUM(BlueprintType)
enum class EWarframeCharacterAILowerState : uint8
{
	Crouching,
	Dead,
	Falling,
	Idle,
	Jumping,
	Sprinting,

	AtCoverStanding,
	AtCoverCrouching,
	AtCoverFiringStanding,
	AtCoverFiringCrouching,
};

UENUM(BlueprintType)
enum class EWarframeCharacterAIUpperState : uint8
{
	Idle,
	Reloading,
	Firing,
	WeaponSwitching,
	Ironsight,
};

UENUM(BlueprintType)
enum class EWarframeCharacterAIActionEvent : uint8
{
	Jump,
	Reload,
	SwitchWeapon,
};

struct FWarframeCharacterAIStateMachineLayer_Lower : public FWarframeCharacterStateMachineLayer_Lower
{
	FStateObject* AtCoverStandingState;
	FStateObject* AtCoverCrouchingState;
	FStateObject* AtCoverFiringStandingState;
	FStateObject* AtCoverFiringCrouchingState;
};

UCLASS()
class WARFRAME_A_API UWarframeCharacterAIStateMachineComponent : public UWarframeCharacterStateMachineComponent
{
	GENERATED_BODY()

public:
	UWarframeCharacterAIStateMachineComponent();

	virtual void Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer)override;

	virtual void ReInit()override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)override;

public:
	FWarframeCharacterAIStateMachineLayer_Lower* LowerLayer;

	UPROPERTY(BlueprintReadOnly)
	class UCoverPoint* CoverPoint;

	int32 OldAtCoverDirection;

	UPROPERTY(BlueprintReadOnly)
	int32 NewAtCoverDirection;

	/** Determines which state to transit to, FiringCrouched or FiringStanding? */
	bool bIsFiringStandingDesired;

private:
	static FWarframeCharacterAILowerState_Idle						LowerIdleState;
	static FWarframeCharacterAILowerState_AtCoverStanding			LowerAtCoverStandingState;
	static FWarframeCharacterAILowerState_AtCoverCrouching			LowerAtCoverCrouchingState;
	static FWarframeCharacterAILowerState_AtCoverFiringStanding		LowerAtCoverFiringStandingState;
	static FWarframeCharacterAILowerState_AtCoverFiringCrouching	LowerAtCoverFiringCrouchingState;
};
