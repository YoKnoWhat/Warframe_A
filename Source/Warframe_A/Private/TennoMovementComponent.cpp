// Fill out your copyright notice in the Description page of Project Settings.

#include "TennoMovementComponent.h"
#include "TennoCharacter.h"


void UTennoMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case static_cast<int32>(ETennoMovementMode::Sliding):
		this->PhysCustomSliding(DeltaTime, Iterations);
		break;
	case static_cast<int32>(ETennoMovementMode::AirGlide) :
		this->PhysCustomAirGlide(DeltaTime, Iterations);
		break;
	default:
		break;
	}
}

void UTennoMovementComponent::PhysCustomSliding(float DeltaTime, int32 Iterations)
{
	this->Acceleration = -Cast<APawn>(this->GetOwner())->GetBaseAimRotation().Vector() * MyAccel * DeltaTime;
	this->Velocity += this->Acceleration * DeltaTime;
}

void UTennoMovementComponent::PhysCustomAirGlide(float DeltaTime, int32 Iterations)
{}