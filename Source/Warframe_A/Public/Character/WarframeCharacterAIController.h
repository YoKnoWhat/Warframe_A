
#pragma once

#include "Runtime/AIModule/Classes/AIController.h"
#include "WarframeCharacterAIController.generated.h"


class UCoverPoint;

UCLASS()
class WARFRAME_A_API AWarframeCharacterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWarframeCharacterAIController(const FObjectInitializer& ObjectInitializer);

	virtual void Possess(APawn* InPawn)override;

	virtual void Tick(float DeltaTime)override;

	UFUNCTION(BlueprintCallable)
	virtual void BeginFire();

	UFUNCTION(BlueprintCallable)
	virtual void StopFire();

	UFUNCTION(BlueprintCallable)
	virtual void Reload();

	UFUNCTION(BlueprintCallable)
	virtual void BeginSprint();

	UFUNCTION(BlueprintCallable)
	virtual void StopSprint();

	UFUNCTION(BlueprintCallable)
	virtual void TakeCover(UCoverPoint* InCoverPoint);

	UFUNCTION(BlueprintCallable)
	virtual void ChangeAtCoverDirection(int32 Direction);

	UFUNCTION(BlueprintCallable)
	virtual void SetDesiredCoverFiringPosture(bool IsFiringStandingDesired);

	UFUNCTION(BlueprintCallable)
	virtual void LeaveCover();

protected:
	UPROPERTY()
	class UTargetSelectionComponent* TargetSelection;

	UPROPERTY(BlueprintReadOnly)
	float PatrolRadius = 300.0f;
};