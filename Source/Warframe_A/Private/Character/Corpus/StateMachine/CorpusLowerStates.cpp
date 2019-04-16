
#include "Character/Corpus/StateMachine/CorpusLowerStates.h"
#include "Character/Corpus/Corpus.h"
#include "Character/Corpus/StateMachine/CorpusStateMachineComponent.h"

#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"


int32 FCorpusLowerState_Crouching::GetID()const
{
	return CastToUnderlyingType(ECorpusLowerState::Crouching);
}

int32 FCorpusLowerState_Crouching::OnUpdate(float DeltaTime)
{
	UCorpusStateMachineComponent* StateMachine = Cast<UCorpusStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsSprinting)
	{
		return CastToUnderlyingType(ECorpusLowerState::Sprinting);
	}
	else if (StateMachine->IsCrouching)
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(ECorpusLowerState::Idle);
	}
}

void FCorpusLowerState_Crouching::OnEnter(int32 StateFromID)
{
	Character->Crouch();
}

void FCorpusLowerState_Crouching::OnExit()
{
	Character->UnCrouch();
}

int32 FCorpusLowerState_Crouching::OnCustomEvent(int32 EventID)
{
	switch (static_cast<ECorpusActionEvent>(EventID))
	{
	case ECorpusActionEvent::Jump:
		return CastToUnderlyingType(ECorpusLowerState::Jumping);
	default:
		return this->GetID();
	}
}



int32 FCorpusLowerState_Falling::GetID()const
{
	return CastToUnderlyingType(ECorpusLowerState::Falling);
}

int32 FCorpusLowerState_Falling::OnUpdate(float DeltaTime)
{
	UCorpusStateMachineComponent* StateMachine = Cast<UCorpusStateMachineComponent>(Character->GetStateMachine());
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(ECorpusLowerState::Idle);
	}
}

void FCorpusLowerState_Falling::OnEnter(int32 StateFromID)
{}

void FCorpusLowerState_Falling::OnExit()
{}

int32 FCorpusLowerState_Falling::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}



int32 FCorpusLowerState_Idle::GetID()const
{
	return CastToUnderlyingType(ECorpusLowerState::Idle);
}

int32 FCorpusLowerState_Idle::OnUpdate(float DeltaTime)
{
	UCorpusStateMachineComponent* StateMachine = Cast<UCorpusStateMachineComponent>(Character->GetStateMachine());
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return CastToUnderlyingType(ECorpusLowerState::Falling);
	}
	else if (StateMachine->IsCrouching)
	{
		return CastToUnderlyingType(ECorpusLowerState::Crouching);
	}
	else if (StateMachine->IsSprinting && Character->GetVelocity().Size2D() > 0.0f)
	{
		return CastToUnderlyingType(ECorpusLowerState::Sprinting);
	}
	else
	{
		return this->GetID();
	}
}

void FCorpusLowerState_Idle::OnEnter(int32 StateFromID)
{}

void FCorpusLowerState_Idle::OnExit()
{}

int32 FCorpusLowerState_Idle::OnCustomEvent(int32 EventID)
{
	switch (static_cast<ECorpusActionEvent>(EventID))
	{
	case ECorpusActionEvent::Jump:
		return CastToUnderlyingType(ECorpusLowerState::Jumping);
	default:
		return this->GetID();
	}
}



int32 FCorpusLowerState_Jumping::GetID()const
{
	return CastToUnderlyingType(ECorpusLowerState::Jumping);
}

int32 FCorpusLowerState_Jumping::OnUpdate(float DeltaTime)
{
	Time += DeltaTime;
	if (Time > 0.5f)
	{
		return CastToUnderlyingType(ECorpusLowerState::Falling);
	}
	else
	{
		return this->GetID();
	}
}

void FCorpusLowerState_Jumping::OnEnter(int32 StateFromID)
{
	Time = 0.0f;

	Character->Jump();
}

void FCorpusLowerState_Jumping::OnExit()
{}

int32 FCorpusLowerState_Jumping::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}



int32 FCorpusLowerState_Sprinting::GetID()const
{
	return CastToUnderlyingType(ECorpusLowerState::Sprinting);
}

int32 FCorpusLowerState_Sprinting::OnUpdate(float DeltaTime)
{
	UCorpusStateMachineComponent* StateMachine = Cast<UCorpusStateMachineComponent>(Character->GetStateMachine());
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return CastToUnderlyingType(ECorpusLowerState::Falling);
	}
	else if (StateMachine->IsCrouching)
	{
		return CastToUnderlyingType(ECorpusLowerState::Crouching);
	}
	else if (StateMachine->IsSprinting == false || Character->GetVelocity().Size2D() == 0.0f)
	{
		return CastToUnderlyingType(ECorpusLowerState::Idle);
	}
	else
	{
		return this->GetID();
	}
}

void FCorpusLowerState_Sprinting::OnEnter(int32 StateFromID)
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	this->MaxWalkSpeedBerore = CharacterMovement->MaxWalkSpeed;
	CharacterMovement->MaxWalkSpeed = 800.0f;
}

void FCorpusLowerState_Sprinting::OnExit()
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement());

	CharacterMovement->MaxWalkSpeed = this->MaxWalkSpeedBerore;
}

int32 FCorpusLowerState_Sprinting::OnCustomEvent(int32 EventID)
{
	switch (static_cast<ECorpusActionEvent>(EventID))
	{
	case ECorpusActionEvent::Jump:
		return CastToUnderlyingType(ECorpusLowerState::Jumping);
	default:
		return this->GetID();
	}
}
