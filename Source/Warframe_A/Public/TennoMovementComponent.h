// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TennoMovementComponent.generated.h"


UENUM(BlueprintType)
enum class ETennoMovementMode : uint8
{
	Sliding, // UMETA(ToolTip = "Sim origin is the location/orientation of the skeletal mesh component."),
	AirGlide,
};

/**
 * TennoMovementComponent handles movement logic for our tenno warriors.
 */
UCLASS( Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class WARFRAME_A_API UTennoMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void PhysCustom(float DeltaTime, int32 Iterations)override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		float MyAccel;
	
protected:
	void PhysCustomSliding(float DeltaTime, int32 Iterations);

	void PhysCustomAirGlide(float DeltaTime, int32 Iterations);
};
