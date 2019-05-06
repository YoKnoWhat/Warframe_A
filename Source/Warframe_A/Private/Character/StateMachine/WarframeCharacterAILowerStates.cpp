
#include "Character/StateMachine/WarframeCharacterAILowerStates.h"
#include "Character/WarframeCharacter.h"
#include "Character/StateMachine/WarframeCharacterAIStateMachineComponent.h"
#include "Gameplay/CoverGenerator/CoverPoint.h"

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
	UWarframeCharacterAIStateMachineComponent* WarframeCharacterAIStateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(StateMachine);

	if (WarframeCharacterAIStateMachine->CoverPoint == nullptr)
	{
		return WarframeCharacterAIStateMachine->LowerLayer->IdleState;
	}
	else if (WarframeCharacterAIStateMachine->UpperLayer->CurrentState->GetID() == CastToUnderlyingType(EWarframeCharacterAIUpperState::Firing))
	{
		if (WarframeCharacterAIStateMachine->bIsFiringStandingDesired)
		{
			return WarframeCharacterAIStateMachine->LowerLayer->AtCoverFiringStandingState;
		}
		else
		{
			return WarframeCharacterAIStateMachine->LowerLayer->AtCoverFiringCrouchingState;
		}
	}
	else
	{
		return this;
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
	UWarframeCharacterAIStateMachineComponent* WarframeCharacterAIStateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(StateMachine);

	if (WarframeCharacterAIStateMachine->CoverPoint == nullptr)
	{
		return WarframeCharacterAIStateMachine->LowerLayer->IdleState;
	}
	else if (WarframeCharacterAIStateMachine->UpperLayer->CurrentState->GetID() == CastToUnderlyingType(EWarframeCharacterAIUpperState::Firing))
	{
		if (WarframeCharacterAIStateMachine->bIsFiringStandingDesired)
		{
			return WarframeCharacterAIStateMachine->LowerLayer->AtCoverFiringStandingState;
		}
		else
		{
			return WarframeCharacterAIStateMachine->LowerLayer->AtCoverFiringCrouchingState;
		}
	}
	else
	{
		return this;
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
