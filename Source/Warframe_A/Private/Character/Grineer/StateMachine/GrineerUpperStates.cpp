
#include "Character/Grineer/StateMachine/GrineerUpperStates.h"
#include "Character/Grineer/Grineer.h"
#include "Character/Grineer/StateMachine/GrineerStateMachineComponent.h"
#include "Weapon/WeaponBase.h"


int32 FGrineerUpperState_Firing::GetID()const
{
	return CastToUnderlyingType(EGrineerUpperState::Firing);
}

int32 FGrineerUpperState_Firing::OnUpdate(float DeltaTime)
{
	UGrineerStateMachineComponent* StateMachine = Cast<UGrineerStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsFiring)
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(EGrineerUpperState::Ironsight);
	}
}

void FGrineerUpperState_Firing::OnEnter(int32 StateFromID)
{
	Character->GetCurrentWeapon()->BeginFire();
}

void FGrineerUpperState_Firing::OnExit()
{
	UGrineerStateMachineComponent* StateMachine = Cast<UGrineerStateMachineComponent>(Character->GetStateMachine());

	Character->GetCurrentWeapon()->StopFire();

	StateMachine->TimeSinceLastFired = 0.0f;
}

int32 FGrineerUpperState_Firing::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EGrineerActionEvent>(EventID))
	{
	case EGrineerActionEvent::Reload:
		return CastToUnderlyingType(EGrineerUpperState::Reloading);
	default:
		return this->GetID();
	}
}



int32 FGrineerUpperState_Idle::GetID()const
{
	return CastToUnderlyingType(EGrineerUpperState::Idle);
}

int32 FGrineerUpperState_Idle::OnUpdate(float DeltaTime)
{
	UGrineerStateMachineComponent* StateMachine = Cast<UGrineerStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->TimeSinceLastFired < 3.0f)
	{
		return CastToUnderlyingType(EGrineerUpperState::Ironsight);
	}
	else if (StateMachine->IsFiring && Character->GetCurrentWeapon()->GetRemainingMagazine() != 0)
	{
		return CastToUnderlyingType(EGrineerUpperState::Firing);
	}
	else
	{
		return this->GetID();
	}
}

void FGrineerUpperState_Idle::OnEnter(int32 StateFromID)
{}

void FGrineerUpperState_Idle::OnExit()
{}

int32 FGrineerUpperState_Idle::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EGrineerActionEvent>(EventID))
	{
	case EGrineerActionEvent::Reload:
		return CastToUnderlyingType(EGrineerUpperState::Reloading);
	default:
		return this->GetID();
	}
}



int32 FGrineerUpperState_Ironsight::GetID()const
{
	return CastToUnderlyingType(EGrineerUpperState::Ironsight);
}

int32 FGrineerUpperState_Ironsight::OnUpdate(float DeltaTime)
{
	UGrineerStateMachineComponent* StateMachine = Cast<UGrineerStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsFiring && Character->GetCurrentWeapon()->GetRemainingMagazine() != 0)
	{
		return CastToUnderlyingType(EGrineerUpperState::Firing);
	}
	else if (StateMachine->TimeSinceLastFired > 3.0f)
	{
		return CastToUnderlyingType(EGrineerUpperState::Idle);
	}
	else
	{
		return this->GetID();
	}
}

void FGrineerUpperState_Ironsight::OnEnter(int32 StateFromID)
{}

void FGrineerUpperState_Ironsight::OnExit()
{}

int32 FGrineerUpperState_Ironsight::OnCustomEvent(int32 EventID)
{
	switch (static_cast<EGrineerActionEvent>(EventID))
	{
	case EGrineerActionEvent::Reload:
		return CastToUnderlyingType(EGrineerUpperState::Reloading);
	default:
		return this->GetID();
	}
}



int32 FGrineerUpperState_Reloading::GetID()const
{
	return CastToUnderlyingType(EGrineerUpperState::Reloading);
}

int32 FGrineerUpperState_Reloading::OnUpdate(float DeltaTime)
{
	if (Character->GetCurrentWeapon()->IsReloading())
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(EGrineerUpperState::Idle);
	}
}

void FGrineerUpperState_Reloading::OnEnter(int32 StateFromID)
{
	Character->GetCurrentWeapon()->BeginReload();
}

void FGrineerUpperState_Reloading::OnExit()
{
	Character->GetCurrentWeapon()->StopReload();
}

int32 FGrineerUpperState_Reloading::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}
