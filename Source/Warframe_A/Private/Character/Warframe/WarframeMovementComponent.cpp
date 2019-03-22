// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Warframe/WarframeMovementComponent.h"
#include "Character/Warframe/Warframe.h"


bool UWarframeMovementComponent::IsFalling()const
{
	return Super::IsFalling()
		|| (MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == CastToUnderlyingType(EWarframeMovementMode::AirGliding))
		|| (MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == CastToUnderlyingType(EWarframeMovementMode::DoubleJumping));
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
{}

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

		this->Acceleration = this->Acceleration.GetUnsafeNormal() * StandToSlideDeltaSpeed / StandToSlideTime * Owner->GetMovementSpeedMultiplier();
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

	if (Acceleration.Size2D() > 100.0)
	{
		float VelocityMagnitude = Velocity.Size();
		Velocity = Acceleration; // Acceleration.Z will always be zero.
		Velocity.Normalize();
		Velocity *= VelocityMagnitude;
	}
}

void UWarframeMovementComponent::PhysCustomDoubleJumping(float DeltaTime, int32 Iterations)
{
	this->PhysFalling(DeltaTime, Iterations);
}
