
#include "Character/Warframe/StateMachine/UpperStates.h"
#include "Character/Warframe/Warframe.h"
#include "Character/Warframe/WarframeMovementComponent.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Weapon/WeaponBase.h"


int32 FUpperState_Firing::GetID()const
{
	return CastToUnderlyingType(EWarframeUpperState::Firing);
}

int32 FUpperState_Firing::OnUpdate(float DeltaTime)
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

void FUpperState_Firing::OnEnter(int32 StateFromID)
{
	Character->GetCurrentWeapon()->BeginFire();
}

void FUpperState_Firing::OnExit()
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Character->GetStateMachine());

	Character->GetCurrentWeapon()->StopFire();

	StateMachine->TimeSinceLastFired = 0.0f;
}

int32 FUpperState_Firing::OnCustomEvent(int32 EventID)
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



int32 FUpperState_Idle::GetID()const
{
	return CastToUnderlyingType(EWarframeUpperState::Idle);
}

int32 FUpperState_Idle::OnUpdate(float DeltaTime)
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

void FUpperState_Idle::OnEnter(int32 StateFromID)
{}

void FUpperState_Idle::OnExit()
{}

int32 FUpperState_Idle::OnCustomEvent(int32 EventID)
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



int32 FUpperState_Ironsight::GetID()const
{
	return CastToUnderlyingType(EWarframeUpperState::Ironsight);
}

int32 FUpperState_Ironsight::OnUpdate(float DeltaTime)
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

void FUpperState_Ironsight::OnEnter(int32 StateFromID)
{}

void FUpperState_Ironsight::OnExit()
{}

int32 FUpperState_Ironsight::OnCustomEvent(int32 EventID)
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



int32 FUpperState_Reloading::GetID()const
{
	return CastToUnderlyingType(EWarframeUpperState::Reloading);
}

int32 FUpperState_Reloading::OnUpdate(float DeltaTime)
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

void FUpperState_Reloading::OnEnter(int32 StateFromID)
{
	Character->GetCurrentWeapon()->BeginReload();
}

void FUpperState_Reloading::OnExit()
{
	Character->GetCurrentWeapon()->StopReload();
}

int32 FUpperState_Reloading::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EWarframeActionEvent>(EventID))
	{
	case EWarframeActionEvent::SwitchWeapon:
		return CastToUnderlyingType(EWarframeUpperState::WeaponSwitching);
	default:
		return this->GetID();
	}
}



int32 FUpperState_WeaponSwitching::GetID()const
{
	return CastToUnderlyingType(EWarframeUpperState::WeaponSwitching);
}

int32 FUpperState_WeaponSwitching::OnUpdate(float DeltaTime)
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

void FUpperState_WeaponSwitching::OnEnter(int32 StateFromID)
{
	Time = 0.0f;
}

void FUpperState_WeaponSwitching::OnExit()
{}

int32 FUpperState_WeaponSwitching::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}
