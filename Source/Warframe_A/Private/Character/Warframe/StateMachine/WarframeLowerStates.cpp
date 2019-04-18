
#include "Character/Warframe/StateMachine/WarframeLowerStates.h"
#include "Character/Warframe/Warframe.h"
#include "Character/Warframe/WarframeMovementComponent.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"


int32 FWarframeLowerState_AimGliding::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::AimGliding);
}

FStateObject* FWarframeLowerState_AimGliding::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(WarframeStateMachine->GetCharacter()->GetCharacterMovement());

	if (CharacterMovement->IsFalling() == false)
	{
		return WarframeStateMachine->LowerLayer->IdleState;
	}
	else if (WarframeStateMachine->IsAiming == false)
	{
		return WarframeStateMachine->LowerLayer->FallingState;
	}
	else
	{
		WarframeStateMachine->GlideTimer -= DeltaTime;
		
		if (WarframeStateMachine->GlideTimer > 0.0f)
		{
			return this;
		}
		else
		{
			return WarframeStateMachine->LowerLayer->FallingState;
		}
	}
}

void FWarframeLowerState_AimGliding::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(StateMachine->GetCharacter()->GetMovementComponent());

	CharacterMovement->GravityScale = 0.05f;
	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Custom, CastToUnderlyingType(EWarframeMovementMode::AirGliding));
}

void FWarframeLowerState_AimGliding::OnExit(UStateMachineComponent* StateMachine)
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(StateMachine->GetCharacter()->GetMovementComponent());

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

FStateObject* FWarframeLowerState_AimGliding::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		if (Cast<UWarframeStateMachineComponent>(WarframeStateMachine->GetCharacter()->GetStateMachine())->HasDoubleJumped)
		{
			return this;
		}
		else
		{
			return WarframeStateMachine->LowerLayer->DoubleJumpingState;
		}
	default:
		return this;
	}
}



int32 FWarframeLowerState_BulletJumping::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::BulletJumping);
}

FStateObject* FWarframeLowerState_BulletJumping::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	WarframeStateMachine->BulletJumpingTimer += DeltaTime;
	if (WarframeStateMachine->BulletJumpingTimer > 0.5f)
	{
		return WarframeStateMachine->LowerLayer->FallingState;
	}
	else
	{
		return this;
	}
}

void FWarframeLowerState_BulletJumping::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	WarframeStateMachine->BulletJumpingTimer = 0.0f;
}

void FWarframeLowerState_BulletJumping::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeLowerState_BulletJumping::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}



int32 FWarframeLowerState_Crouching::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Crouching);
}

FStateObject* FWarframeLowerState_Crouching::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	if (WarframeStateMachine->IsSprinting)
	{
		return WarframeStateMachine->LowerLayer->SprintingState;
	}
	else if (WarframeStateMachine->IsCrouching)
	{
		return this;
	}
	else
	{
		return WarframeStateMachine->LowerLayer->IdleState;
	}
}

void FWarframeLowerState_Crouching::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	StateMachine->GetCharacter()->Crouch();
}

void FWarframeLowerState_Crouching::OnExit(UStateMachineComponent* StateMachine)
{
	StateMachine->GetCharacter()->UnCrouch();
}

FStateObject* FWarframeLowerState_Crouching::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		return WarframeStateMachine->LowerLayer->BulletJumpingState;
	case EWarframeActionEvent::Roll:
		return WarframeStateMachine->LowerLayer->RollingState;
	default:
		return this;
	}
}



int32 FWarframeLowerState_DoubleJumping::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::DoubleJumping);
}

FStateObject* FWarframeLowerState_DoubleJumping::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	WarframeStateMachine->DoubleJumpingTimer += DeltaTime;
	if (WarframeStateMachine->DoubleJumpingTimer > 0.5f)
	{
		return WarframeStateMachine->LowerLayer->FallingState;
	}
	else
	{
		return this;
	}
}

void FWarframeLowerState_DoubleJumping::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(WarframeStateMachine->GetCharacter()->GetMovementComponent());

	WarframeStateMachine->DoubleJumpingTimer = 0.0f;

	WarframeStateMachine->GetCharacter()->Jump();
	WarframeStateMachine->HasDoubleJumped = true;

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Custom, CastToUnderlyingType(EWarframeMovementMode::DoubleJumping));
}

void FWarframeLowerState_DoubleJumping::OnExit(UStateMachineComponent* StateMachine)
{
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(StateMachine->GetCharacter()->GetMovementComponent());

	if (CharacterMovement->IsFalling())
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	// else
	// {
	// 	See UCharacterMovementComponent::ProcessLanded().
	// }
}

FStateObject* FWarframeLowerState_DoubleJumping::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}



int32 FWarframeLowerState_Falling::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Falling);
}

FStateObject* FWarframeLowerState_Falling::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(WarframeStateMachine->GetCharacter()->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		if (WarframeStateMachine->IsAiming && WarframeStateMachine->GlideTimer > 0.0f)
		{
			return WarframeStateMachine->LowerLayer->AimGlidingState;
		}
		else
		{
			return this;
		}
	}
	else
	{
		return WarframeStateMachine->LowerLayer->IdleState;
	}
}

void FWarframeLowerState_Falling::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{}

void FWarframeLowerState_Falling::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeLowerState_Falling::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		if (Cast<UWarframeStateMachineComponent>(WarframeStateMachine->GetCharacter()->GetStateMachine())->HasDoubleJumped)
		{
			return this;
		}
		else
		{
			return WarframeStateMachine->LowerLayer->DoubleJumpingState;
		}
	default:
		return this;
	}
}



int32 FWarframeLowerState_Idle::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Idle);
}

FStateObject* FWarframeLowerState_Idle::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(WarframeStateMachine->GetCharacter()->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return WarframeStateMachine->LowerLayer->FallingState;
	}
	else if (WarframeStateMachine->IsCrouching)
	{
		if (WarframeStateMachine->GetCharacter()->GetVelocity().Size2D() > 200.0f)
		{
			return WarframeStateMachine->LowerLayer->SlidingState;
		}
		else
		{
			return WarframeStateMachine->LowerLayer->CrouchingState;
		}
	}
	else if (WarframeStateMachine->IsSprinting && WarframeStateMachine->GetCharacter()->GetVelocity().Size2D() > 0.0f)
	{
		if (WarframeStateMachine->IsAiming == false)
		{
			return WarframeStateMachine->LowerLayer->SprintingState;
		}
		else
		{
			return this;
		}
	}
	else
	{
		return this;
	}
}

void FWarframeLowerState_Idle::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	Cast<UWarframeStateMachineComponent>(StateMachine->GetCharacter()->GetStateMachine())->HasDoubleJumped = false;
}

void FWarframeLowerState_Idle::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeLowerState_Idle::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		return WarframeStateMachine->LowerLayer->JumpingState;
	case EWarframeActionEvent::Roll:
		return WarframeStateMachine->LowerLayer->RollingState;
	default:
		return this;
	}
}



int32 FWarframeLowerState_Jumping::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Jumping);
}

FStateObject* FWarframeLowerState_Jumping::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	WarframeStateMachine->JumpingTimer += DeltaTime;
	if (WarframeStateMachine->JumpingTimer > 0.5f)
	{
		return WarframeStateMachine->LowerLayer->FallingState;
	}
	else
	{
		return this;
	}
}

void FWarframeLowerState_Jumping::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	WarframeStateMachine->JumpingTimer += 0.0f;

	WarframeStateMachine->IsAiming = false;

	WarframeStateMachine->GetCharacter()->Jump();
}

void FWarframeLowerState_Jumping::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeLowerState_Jumping::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		if (Cast<UWarframeStateMachineComponent>(WarframeStateMachine->GetCharacter()->GetStateMachine())->HasDoubleJumped == false)
		{
			return WarframeStateMachine->LowerLayer->DoubleJumpingState;
		}
		else
		{
			return this;
		}
	default:
		return this;
	}
}



int32 FWarframeLowerState_Rolling::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Rolling);
}

FStateObject* FWarframeLowerState_Rolling::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	WarframeStateMachine->RollingTimer += DeltaTime;
	if (WarframeStateMachine->RollingTimer > 0.5f)
	{
		return WarframeStateMachine->LowerLayer->IdleState;
	}
	else
	{
		return this;
	}
}

void FWarframeLowerState_Rolling::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	WarframeStateMachine->RollingTimer = 0.0f;
}

void FWarframeLowerState_Rolling::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeLowerState_Rolling::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}



int32 FWarframeLowerState_Sliding::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Sliding);
}

FStateObject* FWarframeLowerState_Sliding::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	if (WarframeStateMachine->GetCharacter()->GetVelocity().Size2D() < 10.0f)
	{
		return WarframeStateMachine->LowerLayer->CrouchingState;
	}
	else if (WarframeStateMachine->IsCrouching)
	{
		return this;
	}
	else
	{
		return WarframeStateMachine->LowerLayer->IdleState;
	}
}

void FWarframeLowerState_Sliding::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(WarframeStateMachine->GetCharacter()->GetCharacterMovement());

	WarframeStateMachine->HasDoubleJumped = false;

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Custom, CastToUnderlyingType(EWarframeMovementMode::Sliding));
	WarframeStateMachine->MaxCustomMovementSpeedBefore = CharacterMovement->MaxCustomMovementSpeed;
	CharacterMovement->MaxCustomMovementSpeed = 2000.0f;
	CharacterMovement->GroundFriction = 1.0f;
	CharacterMovement->StandToSlideDuration = 0.0f;
	CharacterMovement->StandToSlideTime = 0.3f;
	CharacterMovement->StandToSlideDeltaSpeed = 450.0f;

	WarframeStateMachine->GetCharacter()->Crouch();
}

void FWarframeLowerState_Sliding::OnExit(UStateMachineComponent* StateMachine)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(WarframeStateMachine->GetCharacter()->GetCharacterMovement());

	StateMachine->GetCharacter()->UnCrouch();

	CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
	CharacterMovement->MaxCustomMovementSpeed = WarframeStateMachine->MaxCustomMovementSpeedBefore;
	CharacterMovement->GroundFriction = 8.0f;
}

FStateObject* FWarframeLowerState_Sliding::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		return WarframeStateMachine->LowerLayer->BulletJumpingState;
	case EWarframeActionEvent::Roll:
		return WarframeStateMachine->LowerLayer->RollingState;
	default:
		return this;
	}
}



int32 FWarframeLowerState_Sprinting::GetID()const
{
	return CastToUnderlyingType(EWarframeLowerState::Sprinting);
}

FStateObject* FWarframeLowerState_Sprinting::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(WarframeStateMachine->GetCharacter()->GetCharacterMovement());

	if (CharacterMovement->IsFalling())
	{
		return WarframeStateMachine->LowerLayer->FallingState;
	}
	else if (WarframeStateMachine->IsCrouching)
	{
		return WarframeStateMachine->LowerLayer->SlidingState;
	}
	else if (WarframeStateMachine->IsAiming || WarframeStateMachine->IsSprinting == false || WarframeStateMachine->GetCharacter()->GetVelocity().Size2D() == 0.0f)
	{
		return WarframeStateMachine->LowerLayer->IdleState;
	}
	else
	{
		return this;
	}
}

void FWarframeLowerState_Sprinting::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(WarframeStateMachine->GetCharacter()->GetCharacterMovement());

	WarframeStateMachine->MaxWalkSpeedBefore = CharacterMovement->MaxWalkSpeed;
	CharacterMovement->MaxWalkSpeed = 800.0f;
}

void FWarframeLowerState_Sprinting::OnExit(UStateMachineComponent* StateMachine)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(WarframeStateMachine->GetCharacter()->GetCharacterMovement());

	CharacterMovement->MaxWalkSpeed = WarframeStateMachine->MaxWalkSpeedBefore;
}

FStateObject* FWarframeLowerState_Sprinting::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Jump:
		return WarframeStateMachine->LowerLayer->JumpingState;
	case EWarframeActionEvent::Roll:
		return WarframeStateMachine->LowerLayer->RollingState;
	default:
		return this;
	}
}
