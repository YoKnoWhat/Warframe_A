
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Character/StateMachineLayerInitializer.h"
#include "Character/StateObject.h"
#include "Character/WarframeCharacter.h"

#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"


FWarframeLowerState_AimGliding		UWarframeStateMachineComponent::LowerAimGlidingState;
FWarframeLowerState_BulletJumping	UWarframeStateMachineComponent::LowerBulletJumpingState;
FWarframeLowerState_Crouching		UWarframeStateMachineComponent::LowerCrouchingState;
FWarframeLowerState_DoubleJumping	UWarframeStateMachineComponent::LowerDoubleJumpingState;
FWarframeLowerState_Falling			UWarframeStateMachineComponent::LowerFallingState;
FWarframeLowerState_Idle			UWarframeStateMachineComponent::LowerIdleState;
FWarframeLowerState_Jumping			UWarframeStateMachineComponent::LowerJumpingState;
FWarframeLowerState_Rolling			UWarframeStateMachineComponent::LowerRollingState;
FWarframeLowerState_Sliding			UWarframeStateMachineComponent::LowerSlidingState;
FWarframeLowerState_Sprinting		UWarframeStateMachineComponent::LowerSprintingState;

FWarframeAimState_Aiming	UWarframeStateMachineComponent::AimAimingState;
FWarframeAimState_Idle		UWarframeStateMachineComponent::AimIdleState;

UWarframeStateMachineComponent::UWarframeStateMachineComponent() :
	Super(),
	GlideTimer(3.0f)
{}

void UWarframeStateMachineComponent::Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer)
{
	LayerInitializer.SetLayer(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeStateMachineLayer_Lower)
		.SetLayer(CastToUnderlyingType(EWarframeStateLayer::Aim), new FWarframeStateMachineLayer_Lower);

	Super::Init(InCharacter, LayerInitializer);

	/** Lower layer */
	LowerLayer = static_cast<FWarframeStateMachineLayer_Lower*>(LayerInitializer.GetLayer(CastToUnderlyingType(EWarframeCharacterStateLayer::Lower)));
	LowerLayer->AimGlidingState = &LowerAimGlidingState;
	LowerLayer->BulletJumpingState = &LowerBulletJumpingState;
	LowerLayer->CrouchingState = &LowerCrouchingState;
	LowerLayer->DoubleJumpingState = &LowerDoubleJumpingState;
	LowerLayer->FallingState = &LowerFallingState;
	LowerLayer->IdleState = &LowerIdleState;
	LowerLayer->JumpingState = &LowerJumpingState;
	LowerLayer->RollingState = &LowerRollingState;
	LowerLayer->SlidingState = &LowerSlidingState;
	LowerLayer->SprintingState = &LowerSprintingState;

	/** Upper layer */

	/**  Aim layer */
	AimLayer = static_cast<FWarframeStateMachineLayer_Aim*>(LayerInitializer.GetLayer(CastToUnderlyingType(EWarframeStateLayer::Aim)));
	AimLayer->AimingState = &AimAimingState;
	AimLayer->IdleState = &AimIdleState;
}

void UWarframeStateMachineComponent::ReInit()
{
	Super::ReInit();

	AimLayer->IsEnabled = true;
	AimLayer->CurrentState = AimLayer->IdleState;
	AimLayer->CurrentState->OnEnter(this, nullptr);
}

void UWarframeStateMachineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->GetCharacter()->GetMovementComponent()->IsFalling() == false)
	{
		GlideTimer = FMath::Min(GlideTimer + DeltaTime, 3.0f);
	}
}
