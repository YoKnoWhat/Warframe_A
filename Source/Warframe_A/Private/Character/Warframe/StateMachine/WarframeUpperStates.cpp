
#include "Character/Warframe/StateMachine/WarframeUpperStates.h"
#include "Character/Warframe/Warframe.h"
#include "Character/Warframe/WarframeMovementComponent.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Weapon/WeaponBase.h"


int32 FWarframeUpperState_Firing::GetID()const
{
	return CastToUnderlyingType(EWarframeUpperState::Firing);
}

int32 FWarframeUpperState_Firing::OnUpdate(float DeltaTime)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsFiring)
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(EWarframeUpperState::Ironsight);
	}
}

void FWarframeUpperState_Firing::OnEnter(int32 StateFromID)
{
	Character->GetCurrentWeapon()->BeginFire();
}

void FWarframeUpperState_Firing::OnExit()
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	Character->GetCurrentWeapon()->StopFire();

	StateMachine->TimeSinceLastFired = 0.0f;
}

int32 FWarframeUpperState_Firing::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Reload:
		return CastToUnderlyingType(EWarframeUpperState::Reloading);
	case EWarframeActionEvent::SwitchWeapon:
		return CastToUnderlyingType(EWarframeUpperState::WeaponSwitching);
	default:
		return this->GetID();
	}
}



int32 FWarframeUpperState_Idle::GetID()const
{
	return CastToUnderlyingType(EWarframeUpperState::Idle);
}

int32 FWarframeUpperState_Idle::OnUpdate(float DeltaTime)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->TimeSinceLastFired < 3.0f)
	{
		return CastToUnderlyingType(EWarframeUpperState::Ironsight);
	}
	else if (StateMachine->IsFiring && Character->GetCurrentWeapon()->GetRemainingMagazine() != 0)
	{
		return CastToUnderlyingType(EWarframeUpperState::Firing);
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeUpperState_Idle::OnEnter(int32 StateFromID)
{}

void FWarframeUpperState_Idle::OnExit()
{}

int32 FWarframeUpperState_Idle::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Reload:
		return CastToUnderlyingType(EWarframeUpperState::Reloading);
	case EWarframeActionEvent::SwitchWeapon:
		return CastToUnderlyingType(EWarframeUpperState::WeaponSwitching);
	default:
		return this->GetID();
	}
}



int32 FWarframeUpperState_Ironsight::GetID()const
{
	return CastToUnderlyingType(EWarframeUpperState::Ironsight);
}

int32 FWarframeUpperState_Ironsight::OnUpdate(float DeltaTime)
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsFiring && Character->GetCurrentWeapon()->GetRemainingMagazine() != 0)
	{
		return CastToUnderlyingType(EWarframeUpperState::Firing);
	}
	else if (StateMachine->TimeSinceLastFired > 3.0f)
	{
		return CastToUnderlyingType(EWarframeUpperState::Idle);
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeUpperState_Ironsight::OnEnter(int32 StateFromID)
{}

void FWarframeUpperState_Ironsight::OnExit()
{}

int32 FWarframeUpperState_Ironsight::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::Reload:
		return CastToUnderlyingType(EWarframeUpperState::Reloading);
	case EWarframeActionEvent::SwitchWeapon:
		return CastToUnderlyingType(EWarframeUpperState::WeaponSwitching);
	default:
		return this->GetID();
	}
}



int32 FWarframeUpperState_Reloading::GetID()const
{
	return CastToUnderlyingType(EWarframeUpperState::Reloading);
}

int32 FWarframeUpperState_Reloading::OnUpdate(float DeltaTime)
{
	if (Character->GetCurrentWeapon()->IsReloading())
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(EWarframeUpperState::Idle);
	}
}

void FWarframeUpperState_Reloading::OnEnter(int32 StateFromID)
{
	Character->GetCurrentWeapon()->BeginReload();
}

void FWarframeUpperState_Reloading::OnExit()
{
	Character->GetCurrentWeapon()->StopReload();
}

int32 FWarframeUpperState_Reloading::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::SwitchWeapon:
		return CastToUnderlyingType(EWarframeUpperState::WeaponSwitching);
	default:
		return this->GetID();
	}
}



int32 FWarframeUpperState_WeaponSwitching::GetID()const
{
	return CastToUnderlyingType(EWarframeUpperState::WeaponSwitching);
}

int32 FWarframeUpperState_WeaponSwitching::OnUpdate(float DeltaTime)
{
	Time += DeltaTime;

	if (Time > 1.0f)
	{
		return CastToUnderlyingType(EWarframeUpperState::Idle);
	}
	else
	{
		return this->GetID();
	}
}

void FWarframeUpperState_WeaponSwitching::OnEnter(int32 StateFromID)
{
	Time = 0.0f;
}

void FWarframeUpperState_WeaponSwitching::OnExit()
{
	Character->SwitchRangedWeapon();
}

int32 FWarframeUpperState_WeaponSwitching::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}
