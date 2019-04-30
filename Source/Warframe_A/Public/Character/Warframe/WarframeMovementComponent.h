// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WarframeMovementComponent.generated.h"


UENUM(BlueprintType)
enum class EWarframeMovementMode : uint8
{
	Sliding, // UMETA(ToolTip = "Sim origin is the location/orientation of the skeletal mesh component."),
	AirGliding,
	DoubleJumping,
};

/**
 * TennoMovementComponent handles movement logic for our tenno warriors.
 */
UCLASS( Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class WARFRAME_A_API UWarframeMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UWarframeMovementComponent(const FObjectInitializer& ObjectInitializer);

	virtual bool IsFalling()const override;

	virtual bool IsMovingOnGround()const override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)override;

	virtual void PhysCustom(float DeltaTime, int32 Iterations)override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float StandToSlideTime;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float StandToSlideDeltaSpeed;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float StandToSlideDuration;

protected:
	void OnEnterSliding();

	void PhysCustomSliding(float DeltaTime, int32 Iterations);

	void OnEnterAirGliding();

	void PhysCustomAirGliding(float DeltaTime, int32 Iterations);

	void OnEnterDoubleJumping();

	void PhysCustomDoubleJumping(float DeltaTime, int32 Iterations);
};
