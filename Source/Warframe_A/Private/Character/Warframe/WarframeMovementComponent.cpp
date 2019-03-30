// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Warframe/WarframeMovementComponent.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Character/Warframe/Warframe.h"


bool UWarframeMovementComponent::IsFalling()const
{
	return Super::IsFalling()
		|| (MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == CastToUnderlyingType(EWarframeMovementMode::AirGliding))
		|| (MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == CastToUnderlyingType(EWarframeMovementMode::DoubleJumping));
}

bool UWarframeMovementComponent::IsMovingOnGround()const
{
	return Super::IsMovingOnGround()
		|| (MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == CastToUnderlyingType(EWarframeMovementMode::Sliding));
}

void UWarframeMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (MovementMode == EMovementMode::MOVE_Custom)
	{
		if (CustomMovementMode == CastToUnderlyingType(EWarframeMovementMode::Sliding))
		{
			this->OnEnterSliding();
		}
		else if (CustomMovementMode == CastToUnderlyingType(EWarframeMovementMode::AirGliding))
		{
			this->OnEnterAirGliding();
		}
		else if (CustomMovementMode == CastToUnderlyingType(EWarframeMovementMode::DoubleJumping))
		{
			this->OnEnterDoubleJumping();
		}
	}
}

void UWarframeMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);

	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	switch (CustomMovementMode)
	{
	case CastToUnderlyingType(EWarframeMovementMode::Sliding):
		this->PhysCustomSliding(DeltaTime, Iterations);
		break;
	case CastToUnderlyingType(EWarframeMovementMode::AirGliding):
		this->PhysCustomAirGliding(DeltaTime, Iterations);
		break;
	case CastToUnderlyingType(EWarframeMovementMode::DoubleJumping):
		this->PhysCustomDoubleJumping(DeltaTime, Iterations);
		break;
	default:
		break;
	}
}

void UWarframeMovementComponent::OnEnterSliding()
{
	// Walking uses only XY velocity, and must be on a walkable floor, with a Base.
	Velocity.Z = 0.f;
	bCrouchMaintainsBaseLocation = true;
	SetGroundMovementMode(MovementMode);

	// make sure we update our new floor/base on initial entry of the walking physics
	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, false);
	AdjustFloorHeight();
	SetBaseFromFloor(CurrentFloor);
}

void UWarframeMovementComponent::PhysCustomSliding(float DeltaTime, int32 Iterations)
{
	StandToSlideDuration += DeltaTime;

	if (StandToSlideDuration > StandToSlideTime)
	{
		this->Acceleration = FVector::ZeroVector;
	}
	else
	{
		AWarframe* Owner = Cast<AWarframe>(this->GetOwner());

		this->Acceleration = this->Acceleration.GetSafeNormal() * StandToSlideDeltaSpeed / StandToSlideTime * Owner->GetMovementSpeedMultiplier();
		// this->Acceleration = Owner->GetActorRotation().Vector() * 600.0f / 0.2f * Owner->GetMovementSpeedMultiplier();
	}

	this->PhysWalking(DeltaTime, Iterations);
}

void UWarframeMovementComponent::OnEnterAirGliding()
{
	this->Velocity *= 0.5f;
}

void UWarframeMovementComponent::PhysCustomAirGliding(float DeltaTime, int32 Iterations)
{
	this->PhysFalling(DeltaTime, Iterations);
}

void UWarframeMovementComponent::OnEnterDoubleJumping()
{
	Velocity.Z = 0.0f;

	if (Acceleration.Size2D() > 100.0f)
	{
		Velocity = Acceleration; // Acceleration.Z will always be zero.
		Velocity.Normalize();
		Velocity *= this->MaxWalkSpeed * Acceleration.Size2D() / this->MaxAcceleration;
	}
}

void UWarframeMovementComponent::PhysCustomDoubleJumping(float DeltaTime, int32 Iterations)
{
	this->PhysFalling(DeltaTime, Iterations);
}
