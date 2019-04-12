
#include "Character/Grineer/StateMachine/GrineerLowerStates.h"
#include "Character/Grineer/Grineer.h"
#include "Character/Grineer/StateMachine/GrineerStateMachineComponent.h"

#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"


int32 FGrineerLowerState_Crouching::GetID()const
{
	return CastToUnderlyingType(EGrineerLowerState::Crouching);
}

int32 FGrineerLowerState_Crouching::OnUpdate(float DeltaTime)
{
	UGrineerStateMachineComponent* StateMachine = Cast<UGrineerStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsSprinting)
	{
		return CastToUnderlyingType(EGrineerLowerState::Sprinting);
	}
	else if (StateMachine->IsCrouching)
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(EGrineerLowerState::Idle);
	}
}

void FGrineerLowerState_Crouching::OnEnter(int32 StateFromID)
{
	Character->Crouch();
}

void FGrineerLowerState_Crouching::OnExit()
{
	Character->UnCrouch();
}

int32 FGrineerLowerState_Crouching::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EGrineerActionEvent>(EventID))
	{
	case EGrineerActionEvent::Jump:
		return CastToUnderlyingType(EGrineerLowerState::Jumping);
	default:
		return this->GetID();
	}
}



int32 FGrineerLowerState_Falling::GetID()const
{
	return CastToUnderlyingType(EGrineerLowerState::Falling);
}

int32 FGrineerLowerState_Falling::OnUpdate(float DeltaTime)
{
	UGrineerStateMachineComponent* StateMachine = Cast<UGrineerStateMachineComponent>(Character->GetStateMachine());
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(EGrineerLowerState::Idle);
	}
}

void FGrineerLowerState_Falling::OnEnter(int32 StateFromID)
{}

void FGrineerLowerState_Falling::OnExit()
{}

int32 FGrineerLowerState_Falling::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}



int32 FGrineerLowerState_Idle::GetID()const
{
	return CastToUnderlyingType(EGrineerLowerState::Idle);
}

int32 FGrineerLowerState_Idle::OnUpdate(float DeltaTime)
{
	UGrineerStateMachineComponent* StateMachine = Cast<UGrineerStateMachineComponent>(Character->GetStateMachine());
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return CastToUnderlyingType(EGrineerLowerState::Falling);
	}
	else if (StateMachine->IsCrouching)
	{
		return CastToUnderlyingType(EGrineerLowerState::Crouching);
	}
	else if (StateMachine->IsSprinting && Character->GetVelocity().Size2D() > 0.0f)
	{
		return CastToUnderlyingType(EGrineerLowerState::Sprinting);
	}
	else
	{
		return this->GetID();
	}
}

void FGrineerLowerState_Idle::OnEnter(int32 StateFromID)
{}

void FGrineerLowerState_Idle::OnExit()
{}

int32 FGrineerLowerState_Idle::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EGrineerActionEvent>(EventID))
	{
	case EGrineerActionEvent::Jump:
		return CastToUnderlyingType(EGrineerLowerState::Jumping);
	default:
		return this->GetID();
	}
}



int32 FGrineerLowerState_Jumping::GetID()const
{
	return CastToUnderlyingType(EGrineerLowerState::Jumping);
}

int32 FGrineerLowerState_Jumping::OnUpdate(float DeltaTime)
{
	Time += DeltaTime;
	if (Time > 0.5f)
	{
		return CastToUnderlyingType(EGrineerLowerState::Falling);
	}
	else
	{
		return this->GetID();
	}
}

void FGrineerLowerState_Jumping::OnEnter(int32 StateFromID)
{
	Time = 0.0f;

	Character->Jump();
}

void FGrineerLowerState_Jumping::OnExit()
{}

int32 FGrineerLowerState_Jumping::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}



int32 FGrineerLowerState_Sprinting::GetID()const
{
	return CastToUnderlyingType(EGrineerLowerState::Sprinting);
}

int32 FGrineerLowerState_Sprinting::OnUpdate(float DeltaTime)
{
	UGrineerStateMachineComponent* StateMachine = Cast<UGrineerStateMachineComponent>(Character->GetStateMachine());
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return CastToUnderlyingType(EGrineerLowerState::Falling);
	}
	else if (StateMachine->IsCrouching)
	{
		return CastToUnderlyingType(EGrineerLowerState::Crouching);
	}
	else if (StateMachine->IsSprinting == false || Character->GetVelocity().Size2D() == 0.0f)
	{
		return CastToUnderlyingType(EGrineerLowerState::Idle);
	}
	else
	{
		return this->GetID();
	}
}

void FGrineerLowerState_Sprinting::OnEnter(int32 StateFromID)
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	this->MaxWalkSpeedBerore = CharacterMovement->MaxWalkSpeed;
	CharacterMovement->MaxWalkSpeed = 800.0f;
}

void FGrineerLowerState_Sprinting::OnExit()
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	CharacterMovement->MaxWalkSpeed = this->MaxWalkSpeedBerore;
}

int32 FGrineerLowerState_Sprinting::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EGrineerActionEvent>(EventID))
	{
	case EGrineerActionEvent::Jump:
		return CastToUnderlyingType(EGrineerLowerState::Jumping);
	default:
		return this->GetID();
	}
}
