
#pragma once

#include "Runtime/AIModule/Classes/AIController.h"
#include "WarframeCharacterAIController.generated.h"


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

protected:
	UPROPERTY()
	class UTargetSelectionComponent* TargetSelection;

	UPROPERTY(BlueprintReadOnly)
	float PatrolRadius = 300.0f;
};