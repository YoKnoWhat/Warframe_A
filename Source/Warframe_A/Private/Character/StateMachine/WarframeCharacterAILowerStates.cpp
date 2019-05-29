
#include "Character/StateMachine/WarframeCharacterAILowerStates.h"
#include "Character/StateMachine/WarframeCharacterAIStateMachineComponent.h"
#include "Character/TargetSelectionComponent.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/CoverGenerator/CoverPoint.h"

#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"


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
		// todo: when we have front aim animation, consider the front cover branch.g
		if (WarframeCharacterAIStateMachine->CoverPoint->bLeftCoverStanding || WarframeCharacterAIStateMachine->CoverPoint->bRightCoverStanding)
		{
			return WarframeCharacterAIStateMachine->LowerLayer->AtCoverStandingState;
		}
		else
		{
			return WarframeCharacterAIStateMachine->LowerLayer->AtCoverCrouchingState;
		}
	}
	else if (WarframeCharacterAIStateMachine->bIsCrouching)
	{
		return WarframeCharacterAIStateMachine->LowerLayer->CrouchingState;
	}
	else if (WarframeCharacterAIStateMachine->bIsSprinting && Character->GetVelocity().Size2D() > 0.0f)
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

void FWarframeCharacterAILowerState_Idle::OnExit(UStateMachineComponent* StateMachine, FStateObject* StateTo)
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



int32 FWarframeCharacterAILowerState_AtCoverStanding::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterAILowerState::AtCoverStanding);
}

FStateObject* FWarframeCharacterAILowerState_AtCoverStanding::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterAIStateMachineComponent* WCASM = Cast<UWarframeCharacterAIStateMachineComponent>(StateMachine);

	if (WCASM->CoverPoint == nullptr)
	{
		return WCASM->LowerLayer->IdleState;
	}
	else
	{
		if (WCASM->OldAtCoverDirection != WCASM->NewAtCoverDirection)
		{
			if (WCASM->NewAtCoverDirection == +1)
			{
				if (WCASM->CoverPoint->bRightCoverStanding)
				{
					WCASM->GetCharacter()->SetActorRotation(WCASM->CoverPoint->RotatorXToWall + FRotator(0.0f, +90.0f, 0.0f));
				}
				else if (WCASM->CoverPoint->bRightCoverCrouched)
				{
					return WCASM->LowerLayer->AtCoverCrouchingState;
				}
			}
			else if (WCASM->NewAtCoverDirection == -1)
			{
				if (WCASM->CoverPoint->bLeftCoverStanding)
				{
					WCASM->GetCharacter()->SetActorRotation(WCASM->CoverPoint->RotatorXToWall + FRotator(0.0f, -90.0f, 0.0f));
				}
				else if (WCASM->CoverPoint->bLeftCoverCrouched)
				{
					return WCASM->LowerLayer->AtCoverCrouchingState;
				}
			}
			WCASM->OldAtCoverDirection = WCASM->NewAtCoverDirection;
		}

		// State transition.
		if (WCASM->UpperLayer->CurrentState->GetID() == CastToUnderlyingType(EWarframeCharacterAIUpperState::Firing))
		{
			if (WCASM->bIsFiringStandingDesired)
			{
				return WCASM->LowerLayer->AtCoverFiringStandingState;
			}
			else
			{
				return WCASM->LowerLayer->AtCoverFiringCrouchingState;
			}
		}
		else
		{
			return this;
		}
	}
}

void FWarframeCharacterAILowerState_AtCoverStanding::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{}

void FWarframeCharacterAILowerState_AtCoverStanding::OnExit(UStateMachineComponent* StateMachine, FStateObject* StateTo)
{}

FStateObject* FWarframeCharacterAILowerState_AtCoverStanding::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}



int32 FWarframeCharacterAILowerState_AtCoverCrouching::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterAILowerState::AtCoverCrouching);
}

FStateObject* FWarframeCharacterAILowerState_AtCoverCrouching::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterAIStateMachineComponent* WCASM = Cast<UWarframeCharacterAIStateMachineComponent>(StateMachine);

	if (WCASM->CoverPoint == nullptr)
	{
		return WCASM->LowerLayer->IdleState;
	}
	else
	{
		if (WCASM->OldAtCoverDirection != WCASM->NewAtCoverDirection)
		{
			if (WCASM->NewAtCoverDirection == +1)
			{
				if (WCASM->CoverPoint->bRightCoverCrouched)
				{
					WCASM->GetCharacter()->SetActorRotation(WCASM->CoverPoint->RotatorXToWall + FRotator(0.0f, +90.0f, 0.0f));
				}
				else if (WCASM->CoverPoint->bRightCoverStanding)
				{
					return WCASM->LowerLayer->AtCoverStandingState;
				}
			}
			else if (WCASM->NewAtCoverDirection == -1)
			{
				if (WCASM->CoverPoint->bLeftCoverCrouched)
				{
					WCASM->GetCharacter()->SetActorRotation(WCASM->CoverPoint->RotatorXToWall + FRotator(0.0f, -90.0f, 0.0f));
				}
				else if (WCASM->CoverPoint->bLeftCoverStanding)
				{
					return WCASM->LowerLayer->AtCoverStandingState;
				}
			}
			WCASM->OldAtCoverDirection = WCASM->NewAtCoverDirection;
		}

		// State transition.
		if (WCASM->UpperLayer->CurrentState->GetID() == CastToUnderlyingType(EWarframeCharacterAIUpperState::Firing))
		{
			if (WCASM->bIsFiringStandingDesired)
			{
				return WCASM->LowerLayer->AtCoverFiringStandingState;
			}
			else
			{
				return WCASM->LowerLayer->AtCoverFiringCrouchingState;
			}
		}
		else
		{
			return this;
		}
	}
}

void FWarframeCharacterAILowerState_AtCoverCrouching::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	if (StateFrom->GetID() != CastToUnderlyingType(EWarframeCharacterAILowerState::AtCoverFiringCrouching))
	{
		StateMachine->GetCharacter()->Crouch();
	}
}

void FWarframeCharacterAILowerState_AtCoverCrouching::OnExit(UStateMachineComponent* StateMachine, FStateObject* StateTo)
{
	if (StateTo->GetID() != CastToUnderlyingType(EWarframeCharacterAILowerState::AtCoverFiringCrouching))
	{
		StateMachine->GetCharacter()->UnCrouch();
	}
}

FStateObject* FWarframeCharacterAILowerState_AtCoverCrouching::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}



int32 FWarframeCharacterAILowerState_AtCoverFiringStanding::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterAILowerState::AtCoverFiringStanding);
}

FStateObject* FWarframeCharacterAILowerState_AtCoverFiringStanding::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterAIStateMachineComponent* WarframeCharacterAIStateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(StateMachine);

	if (WarframeCharacterAIStateMachine->CoverPoint == nullptr)
	{
		return WarframeCharacterAIStateMachine->LowerLayer->IdleState;
	}
	else if (WarframeCharacterAIStateMachine->UpperLayer->CurrentState->GetID() != CastToUnderlyingType(EWarframeCharacterAIUpperState::Firing))
	{
		if (WarframeCharacterAIStateMachine->CoverPoint->bLeftCoverStanding || WarframeCharacterAIStateMachine->CoverPoint->bRightCoverStanding)
		{
			return WarframeCharacterAIStateMachine->LowerLayer->AtCoverStandingState;
		}
		else
		{
			return WarframeCharacterAIStateMachine->LowerLayer->AtCoverCrouchingState;
		}
	}
	else
	{
		return this;
	}
}

void FWarframeCharacterAILowerState_AtCoverFiringStanding::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{}

void FWarframeCharacterAILowerState_AtCoverFiringStanding::OnExit(UStateMachineComponent* StateMachine, FStateObject* StateTo)
{}

FStateObject* FWarframeCharacterAILowerState_AtCoverFiringStanding::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}



int32 FWarframeCharacterAILowerState_AtCoverFiringCrouching::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterAILowerState::AtCoverFiringCrouching);
}

FStateObject* FWarframeCharacterAILowerState_AtCoverFiringCrouching::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterAIStateMachineComponent* WarframeCharacterAIStateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(StateMachine);

	if (WarframeCharacterAIStateMachine->CoverPoint == nullptr)
	{
		return WarframeCharacterAIStateMachine->LowerLayer->IdleState;
	}
	else if (WarframeCharacterAIStateMachine->UpperLayer->CurrentState->GetID() != CastToUnderlyingType(EWarframeCharacterAIUpperState::Firing))
	{
		if (WarframeCharacterAIStateMachine->CoverPoint->bLeftCoverStanding || WarframeCharacterAIStateMachine->CoverPoint->bRightCoverStanding)
		{
			return WarframeCharacterAIStateMachine->LowerLayer->AtCoverStandingState;
		}
		else
		{
			return WarframeCharacterAIStateMachine->LowerLayer->AtCoverCrouchingState;
		}
	}
	else
	{
		return this;
	}
}

void FWarframeCharacterAILowerState_AtCoverFiringCrouching::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	if (StateFrom->GetID() != CastToUnderlyingType(EWarframeCharacterAILowerState::AtCoverCrouching))
	{
		StateMachine->GetCharacter()->Crouch();
	}
}

void FWarframeCharacterAILowerState_AtCoverFiringCrouching::OnExit(UStateMachineComponent* StateMachine, FStateObject* StateTo)
{
	if (StateTo->GetID() != CastToUnderlyingType(EWarframeCharacterAILowerState::AtCoverCrouching))
	{
		StateMachine->GetCharacter()->UnCrouch();
	}
}

FStateObject* FWarframeCharacterAILowerState_AtCoverFiringCrouching::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}
