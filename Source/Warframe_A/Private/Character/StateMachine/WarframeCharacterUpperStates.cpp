
#include "Character/StateMachine/WarframeCharacterUpperStates.h"
#include "Character/StateMachine/WarframeCharacterStateMachineComponent.h"
#include "Character/WarframeCharacter.h"
#include "Weapon/WeaponBase.h"


int32 FWarframeCharacterUpperState_Firing::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterUpperState::Firing);
}

FStateObject* FWarframeCharacterUpperState_Firing::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	if (WarframeCharacterStateMachine->bIsFiring && WarframeCharacterStateMachine->GetCharacter()->GetCurrentWeapon()->GetRemainingMagazine() != 0)
	{
		return this;
	}
	else
	{
		return WarframeCharacterStateMachine->UpperLayer->IronsightState;
	}
}

void FWarframeCharacterUpperState_Firing::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	StateMachine->GetCharacter()->GetCurrentWeapon()->BeginFire();
}

void FWarframeCharacterUpperState_Firing::OnExit(UStateMachineComponent* StateMachine, FStateObject* StateTo)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	WarframeCharacterStateMachine->GetCharacter()->GetCurrentWeapon()->StopFire();

	WarframeCharacterStateMachine->TimeSinceLastFired = 0.0f;
}

FStateObject* FWarframeCharacterUpperState_Firing::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeCharacterActionEvent>(EventID))
	{
	case EWarframeCharacterActionEvent::Reload:
		return WarframeCharacterStateMachine->UpperLayer->ReloadingState;
	case EWarframeCharacterActionEvent::SwitchWeapon:
		return WarframeCharacterStateMachine->UpperLayer->WeaponSwitchingState;
	default:
		return this;
	}
}



int32 FWarframeCharacterUpperState_Idle::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterUpperState::Idle);
}

FStateObject* FWarframeCharacterUpperState_Idle::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	if (WarframeCharacterStateMachine->TimeSinceLastFired < 3.0f)
	{
		return WarframeCharacterStateMachine->UpperLayer->IronsightState;
	}
	else if (WarframeCharacterStateMachine->bIsFiring && WarframeCharacterStateMachine->GetCharacter()->GetCurrentWeapon()->GetRemainingMagazine() != 0)
	{
		return WarframeCharacterStateMachine->UpperLayer->FiringState;
	}
	else
	{
		return this;
	}
}

void FWarframeCharacterUpperState_Idle::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{}

void FWarframeCharacterUpperState_Idle::OnExit(UStateMachineComponent* StateMachine, FStateObject* StateTo)
{}

FStateObject* FWarframeCharacterUpperState_Idle::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeCharacterActionEvent>(EventID))
	{
	case EWarframeCharacterActionEvent::Reload:
		return WarframeCharacterStateMachine->UpperLayer->ReloadingState;
	case EWarframeCharacterActionEvent::SwitchWeapon:
		return WarframeCharacterStateMachine->UpperLayer->WeaponSwitchingState;
	default:
		return this;
	}
}



int32 FWarframeCharacterUpperState_Ironsight::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterUpperState::Ironsight);
}

FStateObject* FWarframeCharacterUpperState_Ironsight::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	if (WarframeCharacterStateMachine->bIsFiring && WarframeCharacterStateMachine->GetCharacter()->GetCurrentWeapon()->GetRemainingMagazine() != 0)
	{
		return WarframeCharacterStateMachine->UpperLayer->FiringState;
	}
	else if (WarframeCharacterStateMachine->TimeSinceLastFired > 3.0f)
	{
		return WarframeCharacterStateMachine->UpperLayer->IdleState;
	}
	else
	{
		return this;
	}
}

void FWarframeCharacterUpperState_Ironsight::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{}

void FWarframeCharacterUpperState_Ironsight::OnExit(UStateMachineComponent* StateMachine, FStateObject* StateTo)
{}

FStateObject* FWarframeCharacterUpperState_Ironsight::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeCharacterActionEvent>(EventID))
	{
	case EWarframeCharacterActionEvent::Reload:
		return WarframeCharacterStateMachine->UpperLayer->ReloadingState;
	case EWarframeCharacterActionEvent::SwitchWeapon:
		return WarframeCharacterStateMachine->UpperLayer->WeaponSwitchingState;
	default:
		return this;
	}
}



int32 FWarframeCharacterUpperState_Reloading::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterUpperState::Reloading);
}

FStateObject* FWarframeCharacterUpperState_Reloading::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	if (WarframeCharacterStateMachine->GetCharacter()->GetCurrentWeapon()->IsReloading())
	{
		return this;
	}
	else
	{
		return WarframeCharacterStateMachine->UpperLayer->IdleState;
	}
}

void FWarframeCharacterUpperState_Reloading::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	StateMachine->GetCharacter()->GetCurrentWeapon()->BeginReload();
}

void FWarframeCharacterUpperState_Reloading::OnExit(UStateMachineComponent* StateMachine, FStateObject* StateTo)
{
	StateMachine->GetCharacter()->GetCurrentWeapon()->StopReload();
}

FStateObject* FWarframeCharacterUpperState_Reloading::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	switch (static_cast<EWarframeCharacterActionEvent>(EventID))
	{
	case EWarframeCharacterActionEvent::SwitchWeapon:
		return WarframeCharacterStateMachine->UpperLayer->WeaponSwitchingState;
	default:
		return this;
	}
}



int32 FWarframeCharacterUpperState_WeaponSwitching::GetID()const
{
	return CastToUnderlyingType(EWarframeCharacterUpperState::WeaponSwitching);
}

FStateObject* FWarframeCharacterUpperState_WeaponSwitching::OnUpdate(UStateMachineComponent* StateMachine, float DeltaTime)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	WarframeCharacterStateMachine->WeaponSwitchTimer += DeltaTime;

	if (WarframeCharacterStateMachine->WeaponSwitchTimer > 1.0f)
	{
		return WarframeCharacterStateMachine->UpperLayer->IdleState;
	}
	else
	{
		return this;
	}
}

void FWarframeCharacterUpperState_WeaponSwitching::OnEnter(UStateMachineComponent* StateMachine, FStateObject* StateFrom)
{
	UWarframeCharacterStateMachineComponent* WarframeCharacterStateMachine = Cast<UWarframeCharacterStateMachineComponent>(StateMachine);

	WarframeCharacterStateMachine->WeaponSwitchTimer = 0.0f;
}

void FWarframeCharacterUpperState_WeaponSwitching::OnExit(UStateMachineComponent* StateMachine, FStateObject* StateTo)
{
	StateMachine->GetCharacter()->SwitchRangedWeapon();
}

FStateObject* FWarframeCharacterUpperState_WeaponSwitching::OnCustomEvent(UStateMachineComponent* StateMachine, int32 EventID)
{
	return this;
}
