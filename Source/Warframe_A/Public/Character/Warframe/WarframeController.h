#pragma once

#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "WarframeController.generated.h"


UCLASS()
class WARFRAME_A_API AWarframeController : public APlayerController
{
	GENERATED_BODY()

public:
	AWarframeController(const FObjectInitializer& ObjectInitializer);

	virtual void Possess(APawn* Pawn)override;

	virtual void UnPossess()override;

	virtual void PlayerTick(float DeltaTime)override;

	virtual void SetupInputComponent()override;

protected:
	/** Action handlers. */
	void OnJumpPressed();

	void OnCrouchPressed();

	void OnCrouchReleased();

	void OnToggleCrouchPressed();

	void OnPrimaryFirePressed();

	void OnPrimaryFireReleased();

	void OnAimPressed();

	void OnAimReleased();

	void OnReloadPressed();

	void OnSwitchWeaponPressed();

	void OnSprintPressed();

	void OnSprintReleased();

	void OnAbilityAPressed();

	void OnAbilityAReleased();

	void OnAbilityBPressed();

	void OnAbilityBReleased();

	void OnAbilityCPressed();

	void OnAbilityCReleased();

	void OnAbilityDPressed();

	void OnAbilityDReleased();

	/** Axis handlers. */
	void OnTurn(float Value);

	void OnLookUp(float Value);

	void OnMoveForward(float Value);

	void OnMoveRight(float Value);

protected:
	UPROPERTY()
	class UTargetSelectionComponent* TargetSelection;

	float RollTimeThreshold = 0.1f;

	FTimerHandle RollTimerHandle;
};