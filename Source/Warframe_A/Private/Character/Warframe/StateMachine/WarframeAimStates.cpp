
#include "Character/Warframe/StateMachine/WarframeAimStates.h"
#include "Character/TargetSelectionComponent.h"
#include "Character/Warframe/Warframe.h"
#include "Character/Warframe/WarframeMovementComponent.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Gameplay/WarframeLevelScriptActor.h"
#include "Weapon/WeaponBase.h"

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Engine/PostProcessVolume.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"


int32 FWarframeAimState_Aiming::GetID()const
{
	return CastToUnderlyingType(EWarframeAimState::Aiming);
}

FStateObject* FWarframeAimState_Aiming::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	AWarframeCharacter* Character = WarframeStateMachine->GetCharacter();
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	// Update depth of field focal distance.
	APostProcessVolume* PostProcessVolume = Cast<AWarframeLevelScriptActor>(StateMachine->GetWorld()->GetLevelScriptActor())->GetUnboundPostProcessVolume();
	if (PostProcessVolume != nullptr)
	{
		UTargetSelectionComponent* TargetSelectionComponent = Cast<UTargetSelectionComponent>(Character->GetController()->GetComponentByClass(UTargetSelectionComponent::StaticClass()));
		const FHitResult& SelectedTarget = TargetSelectionComponent->GetSelectedTarget();
		if (SelectedTarget.Distance == 0.0f)
		{
			PostProcessVolume->Settings.DepthOfFieldFocalDistance = 10000.0f;
		}
		else
		{
			PostProcessVolume->Settings.DepthOfFieldFocalDistance = SelectedTarget.Distance;
		}
	}

	if (WarframeStateMachine->IsAiming)
	{
		WarframeStateMachine->CurveTime = FMath::Clamp(WarframeStateMachine->CurveTime + DeltaTime, 0.0f, 0.1f);
	}
	else
	{
		WarframeStateMachine->CurveTime = FMath::Clamp(WarframeStateMachine->CurveTime - DeltaTime, 0.0f, 0.1f);
	}

	UCameraComponent* Camera = Cast<UCameraComponent>(Character->GetComponentByClass(UCameraComponent::StaticClass()));
	if (Camera != nullptr)
	{
		Camera->FieldOfView = FMath::Lerp(FWarframeConfigSingleton::Instance().FieldOfView, WarframeStateMachine->FOVZoomTo, WarframeStateMachine->CurveTime * 10.0f);
	}

	if (WarframeStateMachine->CurveTime == 0.0f && WarframeStateMachine->IsAiming == false)
	{
		return WarframeStateMachine->AimLayer->IdleState;
	}
	else
	{
		return this;
	}
}

void FWarframeAimState_Aiming::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	AWarframeCharacter* Character = WarframeStateMachine->GetCharacter();
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	WarframeStateMachine->FOVZoomTo = Character->GetCurrentWeapon()->GetZoom() * FWarframeConfigSingleton::Instance().FieldOfView;

	Character->bUseControllerRotationYaw = true;

	WarframeStateMachine->MaxWalkSpeedBefore = CharacterMovement->MaxWalkSpeed;
	CharacterMovement->MaxWalkSpeed = 300.0f;

	APostProcessVolume* PostProcessVolume = Cast<AWarframeLevelScriptActor>(StateMachine->GetWorld()->GetLevelScriptActor())->GetUnboundPostProcessVolume();
	if (PostProcessVolume != nullptr)
	{
		WarframeStateMachine->DepthOfFieldFocalDistance = PostProcessVolume->Settings.DepthOfFieldFocalDistance;
		PostProcessVolume->Settings.DepthOfFieldFstop = 1.0f;
	}
}

void FWarframeAimState_Aiming::OnExit(UStateMachineComponent* StateMachine)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);
	AWarframeCharacter* Character = WarframeStateMachine->GetCharacter();
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	Character->bUseControllerRotationYaw = false;

	CharacterMovement->MaxWalkSpeed = WarframeStateMachine->MaxWalkSpeedBefore;

	APostProcessVolume* PostProcessVolume = Cast<AWarframeLevelScriptActor>(StateMachine->GetWorld()->GetLevelScriptActor())->GetUnboundPostProcessVolume();
	if (PostProcessVolume != nullptr)
	{
		PostProcessVolume->Settings.DepthOfFieldFocalDistance = WarframeStateMachine->DepthOfFieldFocalDistance;
		PostProcessVolume->Settings.DepthOfFieldFstop = 32.0f;
	}
}

FStateObject* FWarframeAimState_Aiming::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::SwitchWeapon:
		return WarframeStateMachine->AimLayer->IdleState;
	default:
		return this;
	}
}



int32 FWarframeAimState_Idle::GetID()const
{
	return CastToUnderlyingType(EWarframeAimState::Idle);
}

FStateObject* FWarframeAimState_Idle::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeStateMachineComponent* WarframeStateMachine = Cast<UWarframeStateMachineComponent>(StateMachine);

	if (WarframeStateMachine->IsAiming)
	{
		return WarframeStateMachine->AimLayer->AimingState;
	}
	else
	{
		return this;
	}
}

void FWarframeAimState_Idle::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{}

void FWarframeAimState_Idle::OnExit(UStateMachineComponent* StateMachine)
{}

FStateObject* FWarframeAimState_Idle::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}
