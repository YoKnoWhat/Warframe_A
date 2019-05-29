
#pragma once

#include "Object.h"
#include "WarframeCommon.h"
#include "CoverPoint.generated.h"


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WARFRAME_A_API UCoverPoint : public UObject
{
	GENERATED_BODY()	
	
public:
	UCoverPoint();

	UCoverPoint(const FObjectInitializer& ObjectInitializer);

	UCoverPoint(const FVector& InLocation, const FVector& InDirectionToWall);

	UFUNCTION(BlueprintCallable)
	bool IsLocationShootable(const FVector& Location);

	AWarframeCharacter* GetOccupant()const;

	UFUNCTION(BlueprintCallable)
	bool IsOccupied()const;

	UFUNCTION(BlueprintCallable)
	void Occupy(AWarframeCharacter* Character);

	UFUNCTION(BlueprintCallable)
	void Release();

	FORCEINLINE FVector DirectionToWall()const
	{
		return _DirectionToWall;
	}

	void DirectionToWall(FVector val);

public: 
	/** location of the cover */
	UPROPERTY(BlueprintReadWrite, Category = "Cover")
	FVector Location;

	/** Direction to wall (Perpendicular to cover) */
	UPROPERTY(BlueprintReadWrite, Category = "Cover")
	FVector _DirectionToWall;

	/** Rotator from X of direction to wall */
	UPROPERTY(BlueprintReadWrite, Category = "Cover")
	FRotator RotatorXToWall;

	/** Is it a Left cover (can lean on left) */
	UPROPERTY(BlueprintReadWrite, Category = "Cover")
	bool bLeftCoverStanding;

	/** Is it a Right cover (can lean on Right) */
	UPROPERTY(BlueprintReadWrite, Category = "Cover")
	bool bRightCoverStanding;

	/** Is it a Left cover (can lean on left) */
	UPROPERTY(BlueprintReadWrite, Category = "Cover")
	bool bLeftCoverCrouched;

	/** Is it a Right cover (can lean on Right) */
	UPROPERTY(BlueprintReadWrite, Category = "Cover")
	bool bRightCoverCrouched;

	/** Can shoot from front crouched (can lean on Right) */
	UPROPERTY(BlueprintReadWrite, Category = "Cover")
	bool bFrontCoverCrouched;

	/** Character to which this point has been given */
	UPROPERTY()
	AWarframeCharacter* OccupiedBy; 
};
