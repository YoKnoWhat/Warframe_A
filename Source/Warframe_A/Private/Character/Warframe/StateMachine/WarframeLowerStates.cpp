
#include "Character/Warframe/StateMachine/WarframeLowerStates.h"
#include "Character/Warframe/Warframe.h"
#include "Character/Warframe/WarframeMovementComponent.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"


int32 FWarframeLowerState_AimGliding::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::AimGliding);
}

int32 FWarframeLowerState_AimGliding::OnUpdate(float DeltaTime)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling() == false)
	{
		return CastToUnderlyingType(EWarframeLowerState::Idle);
	}
	else if (StateMachine->IsAiming == false)
	{
		return CastToUnderlyingType(EWarframeLowerState::Falling);
	}
	else
	{
		StateMachine->GlideTimer -= DeltaTime;
		
		if (StateMachine->GlideTimer > 0.0f)
		{
			return this->GetID();
		}
		else
		{
			return CastToUnderlyingType(EWarframeLowerState::Falling);
		}
	}
}

void FWarframeLowerState_AimGliding::OnEnter(int32 StateFromID)
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());

	CharacterMovement->GravityScale = 0.05f;
	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Custom, CastToUnderlyingType(EWarframeMovementMode::AirGliding));
}

void FWarframeLowerState_AimGliding::OnExit()
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());

	if (CharacterMovement->IsFalling())
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	// else
	// {
	// 	See UCharacterMovementComponent::ProcessLanded().
	// }
	CharacterMovement->GravityScale = 1.0f;
}

int32 FWarframeLowerState_AimGliding::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		if (Cast<UWarframeStateMachineComponent>(Character->GetStateMachine())->HasDoubleJumped)
		{
			return this->GetID();
		}
		else
		{
			return CastToUnderlyingType(EWarframeLowerState::DoubleJumping);
		}
	default:
		return this->GetID();
	}
}



int32 FWarframeLowerState_BulletJumping::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::BulletJumping);
}

int32 FWarframeLowerState_BulletJumping::OnUpdate(float DeltaTime)
{
	Time += DeltaTime;
	if (Time > 0.5f)
	{
		return CastToUnderlyingType(EWarframeLowerState::Falling);
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeLowerState_BulletJumping::OnEnter(int32 StateFromID)
{
	Time = 0.0f;
}

void FWarframeLowerState_BulletJumping::OnExit()
{}

int32 FWarframeLowerState_BulletJumping::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}



int32 FWarframeLowerState_Crouching::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Crouching);
}

int32 FWarframeLowerState_Crouching::OnUpdate(float DeltaTime)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsSprinting)
	{
		return CastToUnderlyingType(EWarframeLowerState::Sprinting);
	}
	else if (StateMachine->IsCrouching)
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(EWarframeLowerState::Idle);
	}
}

void FWarframeLowerState_Crouching::OnEnter(int32 StateFromID)
{
	Character->Crouch();
}

void FWarframeLowerState_Crouching::OnExit()
{
	Character->UnCrouch();
}

int32 FWarframeLowerState_Crouching::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		return CastToUnderlyingType(EWarframeLowerState::BulletJumping);
	case EWarframeActionEvent::Roll:
		return CastToUnderlyingType(EWarframeLowerState::Rolling);
	default:
		return this->GetID();
	}
}



int32 FWarframeLowerState_DoubleJumping::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::DoubleJumping);
}

int32 FWarframeLowerState_DoubleJumping::OnUpdate(float DeltaTime)
{
	Time += DeltaTime;
	if (Time > 0.5f)
	{
		return CastToUnderlyingType(EWarframeLowerState::Falling);
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeLowerState_DoubleJumping::OnEnter(int32 StateFromID)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());

	Time = 0.0f;

	Character->Jump();
	StateMachine->HasDoubleJumped = true;

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Custom, CastToUnderlyingType(EWarframeMovementMode::DoubleJumping));
}

void FWarframeLowerState_DoubleJumping::OnExit()
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());

	if (CharacterMovement->IsFalling())
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	// else
	// {
	// 	See UCharacterMovementComponent::ProcessLanded().
	// }
}

int32 FWarframeLowerState_DoubleJumping::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}



int32 FWarframeLowerState_Falling::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Falling);
}

int32 FWarframeLowerState_Falling::OnUpdate(float DeltaTime)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		if (StateMachine->IsAiming && StateMachine->GlideTimer > 0.0f)
		{
			return CastToUnderlyingType(EWarframeLowerState::AimGliding);
		}
		else
		{
			return this->GetID();
		}
	}
	else
	{
		return CastToUnderlyingType(EWarframeLowerState::Idle);
	}
}

void FWarframeLowerState_Falling::OnEnter(int32 StateFromID)
{}

void FWarframeLowerState_Falling::OnExit()
{}

int32 FWarframeLowerState_Falling::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		if (Cast<UWarframeStateMachineComponent>(Character->GetStateMachine())->HasDoubleJumped)
		{
			return this->GetID();
		}
		else
		{
			return CastToUnderlyingType(EWarframeLowerState::DoubleJumping);
		}
	default:
		return this->GetID();
	}
}



int32 FWarframeLowerState_Idle::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Idle);
}

int32 FWarframeLowerState_Idle::OnUpdate(float DeltaTime)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return CastToUnderlyingType(EWarframeLowerState::Falling);
	}
	else if (StateMachine->IsCrouching)
	{
		if (Character->GetVelocity().Size2D() > 200.0f)
		{
			return CastToUnderlyingType(EWarframeLowerState::Sliding);
		}
		else
		{
			return CastToUnderlyingType(EWarframeLowerState::Crouching);
		}
	}
	else if (StateMachine->IsSprinting && Character->GetVelocity().Size2D() > 0.0f)
	{
		if (StateMachine->IsAiming == false)
		{
			return CastToUnderlyingType(EWarframeLowerState::Sprinting);
		}
		else
		{
			return this->GetID();
		}
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeLowerState_Idle::OnEnter(int32 StateFromID)
{
	Cast<UWarframeStateMachineComponent>(Character->GetStateMachine())->HasDoubleJumped = false;
}

void FWarframeLowerState_Idle::OnExit()
{}

int32 FWarframeLowerState_Idle::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		return CastToUnderlyingType(EWarframeLowerState::Jumping);
	case EWarframeActionEvent::Roll:
		return CastToUnderlyingType(EWarframeLowerState::Rolling);
	default:
		return this->GetID();
	}
}



int32 FWarframeLowerState_Jumping::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Jumping);
}

int32 FWarframeLowerState_Jumping::OnUpdate(float DeltaTime)
{
	Time += DeltaTime;
	if (Time > 0.5f)
	{
		return CastToUnderlyingType(EWarframeLowerState::Falling);
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeLowerState_Jumping::OnEnter(int32 StateFromID)
{
	Time = 0.0f;

	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	StateMachine->IsAiming = false;

	Character->Jump();
}

void FWarframeLowerState_Jumping::OnExit()
{}

int32 FWarframeLowerState_Jumping::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		if (Cast<UWarframeStateMachineComponent>(Character->GetStateMachine())->HasDoubleJumped == false)
		{
			return CastToUnderlyingType(EWarframeLowerState::DoubleJumping);
		}
		else
		{
			return this->GetID();
		}
	default:
		return this->GetID();
	}
}



int32 FWarframeLowerState_Rolling::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Rolling);
}

int32 FWarframeLowerState_Rolling::OnUpdate(float DeltaTime)
{
	Time += DeltaTime;
	if (Time > 0.5f)
	{
		return CastToUnderlyingType(EWarframeLowerState::Idle);
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeLowerState_Rolling::OnEnter(int32 StateFromID)
{
	Time = 0.0f;
}

void FWarframeLowerState_Rolling::OnExit()
{}

int32 FWarframeLowerState_Rolling::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}



int32 FWarframeLowerState_Sliding::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Sliding);
}

int32 FWarframeLowerState_Sliding::OnUpdate(float DeltaTime)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	if (Character->GetVelocity().Size2D() < 10.0f)
	{
		return CastToUnderlyingType(EWarframeLowerState::Crouching);
	}
	else if (StateMachine->IsCrouching)
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(EWarframeLowerState::Idle);
	}
}

void FWarframeLowerState_Sliding::OnEnter(int32 StateFromID)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	StateMachine->HasDoubleJumped = false;

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Custom, CastToUnderlyingType(EWarframeMovementMode::Sliding));
	MaxCustomMovementSpeedBefore = CharacterMovement->MaxCustomMovementSpeed;
	CharacterMovement->MaxCustomMovementSpeed = 2000.0f;
	CharacterMovement->GroundFriction = 1.0f;
	CharacterMovement->StandToSlideDuration = 0.0f;
	CharacterMovement->StandToSlideTime = 0.3f;
	CharacterMovement->StandToSlideDeltaSpeed = 450.0f;

	Character->Crouch();
}

void FWarframeLowerState_Sliding::OnExit()
{
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	Character->UnCrouch();

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
	CharacterMovement->MaxCustomMovementSpeed = MaxCustomMovementSpeedBefore;
	CharacterMovement->GroundFriction = 8.0f;
}

int32 FWarframeLowerState_Sliding::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		return CastToUnderlyingType(EWarframeLowerState::BulletJumping);
	case EWarframeActionEvent::Roll:
		return CastToUnderlyingType(EWarframeLowerState::Rolling);
	default:
		return this->GetID();
	}
}



int32 FWarframeLowerState_Sprinting::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Sprinting);
}

int32 FWarframeLowerState_Sprinting::OnUpdate(float DeltaTime)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return CastToUnderlyingType(EWarframeLowerState::Falling);
	}
	else if (StateMachine->IsCrouching)
	{
		return CastToUnderlyingType(EWarframeLowerState::Sliding);
	}
	else if (StateMachine->IsAiming || StateMachine->IsSprinting == false || Character->GetVelocity().Size2D() == 0.0f)
	{
		return CastToUnderlyingType(EWarframeLowerState::Idle);
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeLowerState_Sprinting::OnEnter(int32 StateFromID)
{
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	this->MaxWalkSpeedBerore = CharacterMovement->MaxWalkSpeed;
	CharacterMovement->MaxWalkSpeed = 800.0f;
}

void FWarframeLowerState_Sprinting::OnExit()
{
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	CharacterMovement->MaxWalkSpeed = this->MaxWalkSpeedBerore;
}

int32 FWarframeLowerState_Sprinting::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		return CastToUnderlyingType(EWarframeLowerState::Jumping);
	case EWarframeActionEvent::Roll:
		return CastToUnderlyingType(EWarframeLowerState::Rolling);
	default:
		return this->GetID();
	}
}
