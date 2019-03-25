
#include "Character/Warframe/StateMachine/LowerStates.h"
#include "Character/Warframe/Warframe.h"
#include "Character/Warframe/WarframeMovementComponent.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"


int32 FLowerState_AimGliding::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::AimGliding);
}

int32 FLowerState_AimGliding::OnUpdate(float DeltaTime)
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

void FLowerState_AimGliding::OnEnter(int32 StateFromID)
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());

	CharacterMovement->GravityScale = 0.05f;
	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Custom, CastToUnderlyingType(EWarframeMovementMode::AirGliding));
}

void FLowerState_AimGliding::OnExit()
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

int32 FLowerState_AimGliding::OnCustomEvent(int32 EventID)
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



int32 FLowerState_BulletJumping::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::BulletJumping);
}

int32 FLowerState_BulletJumping::OnUpdate(float DeltaTime)
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

void FLowerState_BulletJumping::OnEnter(int32 StateFromID)
{
	Time = 0.0f;
}

void FLowerState_BulletJumping::OnExit()
{}

int32 FLowerState_BulletJumping::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}



int32 FLowerState_Crouching::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Crouching);
}

int32 FLowerState_Crouching::OnUpdate(float DeltaTime)
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

void FLowerState_Crouching::OnEnter(int32 StateFromID)
{
	Character->Crouch();
}

void FLowerState_Crouching::OnExit()
{
	Character->UnCrouch();
}

int32 FLowerState_Crouching::OnCustomEvent(int32 EventID)
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



int32 FLowerState_DoubleJumping::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::DoubleJumping);
}

int32 FLowerState_DoubleJumping::OnUpdate(float DeltaTime)
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

void FLowerState_DoubleJumping::OnEnter(int32 StateFromID)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());

	Time = 0.0f;

	Character->Jump();
	StateMachine->HasDoubleJumped = true;

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Custom, CastToUnderlyingType(EWarframeMovementMode::DoubleJumping));
}

void FLowerState_DoubleJumping::OnExit()
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

int32 FLowerState_DoubleJumping::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}



int32 FLowerState_Falling::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Falling);
}

int32 FLowerState_Falling::OnUpdate(float DeltaTime)
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

void FLowerState_Falling::OnEnter(int32 StateFromID)
{}

void FLowerState_Falling::OnExit()
{}

int32 FLowerState_Falling::OnCustomEvent(int32 EventID)
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



int32 FLowerState_Idle::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Idle);
}

int32 FLowerState_Idle::OnUpdate(float DeltaTime)
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

void FLowerState_Idle::OnEnter(int32 StateFromID)
{
	Cast<UWarframeStateMachineComponent>(Character->GetStateMachine())->HasDoubleJumped = false;
}

void FLowerState_Idle::OnExit()
{}

int32 FLowerState_Idle::OnCustomEvent(int32 EventID)
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



int32 FLowerState_Jumping::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Jumping);
}

int32 FLowerState_Jumping::OnUpdate(float DeltaTime)
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

void FLowerState_Jumping::OnEnter(int32 StateFromID)
{
	Time = 0.0f;

	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	StateMachine->IsAiming = false;

	Character->Jump();
}

void FLowerState_Jumping::OnExit()
{}

int32 FLowerState_Jumping::OnCustomEvent(int32 EventID)
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



int32 FLowerState_Rolling::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Rolling);
}

int32 FLowerState_Rolling::OnUpdate(float DeltaTime)
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

void FLowerState_Rolling::OnEnter(int32 StateFromID)
{
	Time = 0.0f;
}

void FLowerState_Rolling::OnExit()
{}

int32 FLowerState_Rolling::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}



int32 FLowerState_Sliding::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Sliding);
}

int32 FLowerState_Sliding::OnUpdate(float DeltaTime)
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

void FLowerState_Sliding::OnEnter(int32 StateFromID)
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
}

void FLowerState_Sliding::OnExit()
{
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
	CharacterMovement->MaxCustomMovementSpeed = MaxCustomMovementSpeedBefore;
	CharacterMovement->GroundFriction = 8.0f;
}

int32 FLowerState_Sliding::OnCustomEvent(int32 EventID)
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



int32 FLowerState_Sprinting::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Sprinting);
}

int32 FLowerState_Sprinting::OnUpdate(float DeltaTime)
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

void FLowerState_Sprinting::OnEnter(int32 StateFromID)
{
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	this->MaxWalkSpeedBerore = CharacterMovement->MaxWalkSpeed;
	CharacterMovement->MaxWalkSpeed = 800.0f;
}

void FLowerState_Sprinting::OnExit()
{
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	CharacterMovement->MaxWalkSpeed = this->MaxWalkSpeedBerore;
}

int32 FLowerState_Sprinting::OnCustomEvent(int32 EventID)
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
