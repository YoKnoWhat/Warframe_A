
#include "Character/Warframe/StateMachine/WarframeAimStates.h"
#include "Character/Warframe/Warframe.h"
#include "Character/Warframe/WarframeMovementComponent.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Weapon/WeaponBase.h"

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"


int32 FWarframeAimState_Aiming::GetID()const
{
	return CastToUnderlyingType(EWarframeAimState::Aiming);
}

int32 FWarframeAimState_Aiming::OnUpdate(float DeltaTime)
{
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsAiming)
	{
		CurveTime = FMath::Clamp(CurveTime + DeltaTime, 0.0f, 0.1f);
	}
	else
	{
		CurveTime = FMath::Clamp(CurveTime - DeltaTime, 0.0f, 0.1f);
	}

	UCameraComponent* Camera = Cast<UCameraComponent>(Character->GetComponentByClass(UCameraComponent::StaticClass()));
	if (Camera != nullptr)
	{
		Camera->FieldOfView = FMath::Lerp(FWarframeConfigSingleton::Instance().FieldOfView, FOVZoomTo, CurveTime * 10.0f);
	}

	if (CurveTime == 0.0f && StateMachine->IsAiming == false)
	{
		return CastToUnderlyingType(EWarframeAimState::Idle);
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeAimState_Aiming::OnEnter(int32 StateFromID)
{
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	this->FOVZoomTo = Character->GetCurrentWeapon()->GetZoom() * FWarframeConfigSingleton::Instance().FieldOfView;

	Character->bUseControllerRotationYaw = true;

	MaxWalkSpeedBefore = CharacterMovement->MaxWalkSpeed;
	CharacterMovement->MaxWalkSpeed = 300.0f;

}

void FWarframeAimState_Aiming::OnExit()
{
	UWarframeMovementComponent* CharacterMovement = Cast<UWarframeMovementComponent>(Character->GetCharacterMovement());

	Character->bUseControllerRotationYaw = false;

	CharacterMovement->MaxWalkSpeed = MaxWalkSpeedBefore;
}

int32 FWarframeAimState_Aiming::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::SwitchWeapon:
		return CastToUnderlyingType(EWarframeLowerState::Idle);
	default:
		return this->GetID();
	}
}



int32 FWarframeAimState_Idle::GetID()const
{
	return CastToUnderlyingType(EWarframeAimState::Idle);
}

int32 FWarframeAimState_Idle::OnUpdate(float DeltaTime)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsAiming)
	{
		return CastToUnderlyingType(EWarframeAimState::Aiming);
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeAimState_Idle::OnEnter(int32 StateFromID)
{}

void FWarframeAimState_Idle::OnExit()
{}

int32 FWarframeAimState_Idle::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}
