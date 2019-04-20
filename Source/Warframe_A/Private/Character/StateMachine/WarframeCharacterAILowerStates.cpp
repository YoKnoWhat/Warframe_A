
#include "Character/StateMachine/WarframeCharacterAILowerStates.h"
#include "Character/WarframeCharacter.h"
#include "Character/StateMachine/WarframeCharacterAIStateMachineComponent.h"

#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"


int32 FWarframeCharacterAILowerState_Idle::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterAILowerState::Idle);
}

FStateObject* FWarframeCharacterAILowerState_Idle::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterAIStateMachineComponent* WarframeCharacterAIStateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(StateMachine);
	AWarframeCharacter* Character = WarframeCharacterAIStateMachine->GetCharacter();
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return WarframeCharacterAIStateMachine->LowerLayer->FallingState;
	}
	else if (WarframeCharacterAIStateMachine->CoverPoint != nullptr)
	{
		return WarframeCharacterAIStateMachine->LowerLayer->AtCoverState;
	}
	else if (WarframeCharacterAIStateMachine->IsCrouching)
	{
		return WarframeCharacterAIStateMachine->LowerLayer->CrouchingState;
	}
	else if (WarframeCharacterAIStateMachine->IsSprinting && Character->GetVelocity().Size2D() > 0.0f)
	{
		return WarframeCharacterAIStateMachine->LowerLayer->SprintingState;
	}
	else
	{
		return this;
	}
}

void FWarframeCharacterAILowerState_Idle::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
}

void FWarframeCharacterAILowerState_Idle::OnExit(UStateMachineComponent* StateMachine)
{
}

FStateObject* FWarframeCharacterAILowerState_Idle::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeCharacterAIStateMachineComponent* WarframeCharacterAIStateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeCharacterActionEvent>(EventID))
	{
	case EWarframeCharacterActionEvent::Jump:
		return WarframeCharacterAIStateMachine->LowerLayer->JumpingState;
	default:
		return this;
	}
}



int32 FWarframeCharacterAILowerState_AtCover::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterAILowerState::AtCover);
}

FStateObject* FWarframeCharacterAILowerState_AtCover::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterAIStateMachineComponent* WarframeCharacterAIStateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(StateMachine);

	if (WarframeCharacterAIStateMachine->CoverPoint == nullptr)
	{
		return WarframeCharacterAIStateMachine->LowerLayer->IdleState;
	}
	else
	{
		return this;
	}
}

void FWarframeCharacterAILowerState_AtCover::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{}

void FWarframeCharacterAILowerState_AtCover::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeCharacterAILowerState_AtCover::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}
