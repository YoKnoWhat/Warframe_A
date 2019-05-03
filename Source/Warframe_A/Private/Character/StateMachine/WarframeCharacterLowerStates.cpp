
#include "Character/StateMachine/WarframeCharacterLowerStates.h"
#include "Character/WarframeCharacter.h"
#include "Character/StateMachine/WarframeCharacterStateMachineComponent.h"

#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"


int32 FWarframeCharacterLowerState_Crouching::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterLowerState::Crouching);
}

FStateObject* FWarframeCharacterLowerState_Crouching::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	if (WarframeCharacterStateMachine->bIsSprinting)
	{
		return WarframeCharacterStateMachine->LowerLayer->SprintingState;
	}
	else if (WarframeCharacterStateMachine->bIsCrouching)
	{
		return this;
	}
	else
	{
		return WarframeCharacterStateMachine->LowerLayer->IdleState;
	}
}

void FWarframeCharacterLowerState_Crouching::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	StateMachine->GetCharacter()->Crouch();
}

void FWarframeCharacterLowerState_Crouching::OnExit(UStateMachineComponent* StateMachine)
{
	StateMachine->GetCharacter()->UnCrouch();
}

FStateObject* FWarframeCharacterLowerState_Crouching::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeCharacterActionEvent>(EventID))
	{
	case EWarframeCharacterActionEvent::Jump:
		return WarframeCharacterStateMachine->LowerLayer->JumpingState;
	default:
		return this;
	}
}



int32 FWarframeCharacterLowerState_Dead::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterLowerState::Dead);
}

FStateObject* FWarframeCharacterLowerState_Dead::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	return this;
}

void FWarframeCharacterLowerState_Dead::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{}

void FWarframeCharacterLowerState_Dead::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeCharacterLowerState_Dead::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}



int32 FWarframeCharacterLowerState_Falling::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterLowerState::Falling);
}

FStateObject* FWarframeCharacterLowerState_Falling::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(WarframeCharacterStateMachine->GetCharacter()->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return this;
	}
	else
	{
		return WarframeCharacterStateMachine->LowerLayer->IdleState;
	}
}

void FWarframeCharacterLowerState_Falling::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{}

void FWarframeCharacterLowerState_Falling::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeCharacterLowerState_Falling::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}



int32 FWarframeCharacterLowerState_Idle::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterLowerState::Idle);
}

FStateObject* FWarframeCharacterLowerState_Idle::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);
	AWarframeCharacter* Character = WarframeCharacterStateMachine->GetCharacter();
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());
	
	if (CharacterMovement->IsFalling())
	{
		return WarframeCharacterStateMachine->LowerLayer->FallingState;
	}
	else if (WarframeCharacterStateMachine->bIsCrouching)
	{
		return WarframeCharacterStateMachine->LowerLayer->CrouchingState;
	}
	else if (WarframeCharacterStateMachine->bIsSprinting && Character->GetVelocity().Size2D() > 0.0f)
	{
		return WarframeCharacterStateMachine->LowerLayer->SprintingState;
	}
	else
	{
		return this;
	}
}

void FWarframeCharacterLowerState_Idle::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{}

void FWarframeCharacterLowerState_Idle::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeCharacterLowerState_Idle::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeCharacterActionEvent>(EventID))
	{
	case EWarframeCharacterActionEvent::Jump:
		return WarframeCharacterStateMachine->LowerLayer->JumpingState;
	default:
		return this;
	}
}



int32 FWarframeCharacterLowerState_Jumping::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterLowerState::Jumping);
}

FStateObject* FWarframeCharacterLowerState_Jumping::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	WarframeCharacterStateMachine->JumpingTimer += DeltaTime;
	if (WarframeCharacterStateMachine->JumpingTimer > 0.5f)
	{
		return WarframeCharacterStateMachine->LowerLayer->FallingState;
	}
	else
	{
		return this;
	}
}

void FWarframeCharacterLowerState_Jumping::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	WarframeCharacterStateMachine->JumpingTimer = 0.0f;

	WarframeCharacterStateMachine->GetCharacter()->Jump();
}

void FWarframeCharacterLowerState_Jumping::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeCharacterLowerState_Jumping::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}



int32 FWarframeCharacterLowerState_Sprinting::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterLowerState::Sprinting);
}

FStateObject* FWarframeCharacterLowerState_Sprinting::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);
	AWarframeCharacter* Character = WarframeCharacterStateMachine->GetCharacter();
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return WarframeCharacterStateMachine->LowerLayer->FallingState;
	}
	else if (WarframeCharacterStateMachine->bIsCrouching)
	{
		return WarframeCharacterStateMachine->LowerLayer->CrouchingState;
	}
	else if (WarframeCharacterStateMachine->bIsSprinting == false || Character->GetVelocity().Size2D() == 0.0f)
	{
		return WarframeCharacterStateMachine->LowerLayer->IdleState;
	}
	else
	{
		return this;
	}
}

void FWarframeCharacterLowerState_Sprinting::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(WarframeCharacterStateMachine->GetCharacter()->GetCharacterMovement());

	WarframeCharacterStateMachine->MaxWalkSpeedBefore = CharacterMovement->MaxWalkSpeed;
	CharacterMovement->MaxWalkSpeed = 800.0f;
}

void FWarframeCharacterLowerState_Sprinting::OnExit(UStateMachineComponent* StateMachine)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(WarframeCharacterStateMachine->GetCharacter()->GetCharacterMovement());

	CharacterMovement->MaxWalkSpeed = WarframeCharacterStateMachine->MaxWalkSpeedBefore;
}

FStateObject* FWarframeCharacterLowerState_Sprinting::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeCharacterActionEvent>(EventID))
	{
	case EWarframeCharacterActionEvent::Jump:
		return WarframeCharacterStateMachine->LowerLayer->JumpingState;
	default:
		return this;
	}
}
