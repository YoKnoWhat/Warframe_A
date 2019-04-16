
#include "Character/Corpus/StateMachine/CorpusUpperStates.h"
#include "Character/Corpus/Corpus.h"
#include "Character/Corpus/StateMachine/CorpusStateMachineComponent.h"
#include "Weapon/WeaponBase.h"


int32 FCorpusUpperState_Firing::GetID()const
{
	return CastToUnderlyingType(ECorpusUpperState::Firing);
}

int32 FCorpusUpperState_Firing::OnUpdate(float DeltaTime)
{
	UCorpusStateMachineComponent* StateMachine = Cast<UCorpusStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsFiring)
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(ECorpusUpperState::Ironsight);
	}
}

void FCorpusUpperState_Firing::OnEnter(int32 StateFromID)
{
	Character->GetCurrentWeapon()->BeginFire();
}

void FCorpusUpperState_Firing::OnExit()
{
	UCorpusStateMachineComponent* StateMachine = Cast<UCorpusStateMachineComponent>(Character->GetStateMachine());

	Character->GetCurrentWeapon()->StopFire();

	StateMachine->TimeSinceLastFired = 0.0f;
}

int32 FCorpusUpperState_Firing::OnCustomEvent(int32 EventID)
{
	switch (static_cast<ECorpusActionEvent>(EventID))
	{
	case ECorpusActionEvent::Reload:
		return CastToUnderlyingType(ECorpusUpperState::Reloading);
	default:
		return this->GetID();
	}
}



int32 FCorpusUpperState_Idle::GetID()const
{
	return CastToUnderlyingType(ECorpusUpperState::Idle);
}

int32 FCorpusUpperState_Idle::OnUpdate(float DeltaTime)
{
	UCorpusStateMachineComponent* StateMachine = Cast<UCorpusStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->TimeSinceLastFired < 3.0f)
	{
		return CastToUnderlyingType(ECorpusUpperState::Ironsight);
	}
	else if (StateMachine->IsFiring && Character->GetCurrentWeapon()->GetRemainingMagazine() != 0)
	{
		return CastToUnderlyingType(ECorpusUpperState::Firing);
	}
	else
	{
		return this->GetID();
	}
}

void FCorpusUpperState_Idle::OnEnter(int32 StateFromID)
{}

void FCorpusUpperState_Idle::OnExit()
{}

int32 FCorpusUpperState_Idle::OnCustomEvent(int32 EventID)
{
	switch (static_cast<ECorpusActionEvent>(EventID))
	{
	case ECorpusActionEvent::Reload:
		return CastToUnderlyingType(ECorpusUpperState::Reloading);
	default:
		return this->GetID();
	}
}



int32 FCorpusUpperState_Ironsight::GetID()const
{
	return CastToUnderlyingType(ECorpusUpperState::Ironsight);
}

int32 FCorpusUpperState_Ironsight::OnUpdate(float DeltaTime)
{
	UCorpusStateMachineComponent* StateMachine = Cast<UCorpusStateMachineComponent>(Character->GetStateMachine());

	if (StateMachine->IsFiring && Character->GetCurrentWeapon()->GetRemainingMagazine() != 0)
	{
		return CastToUnderlyingType(ECorpusUpperState::Firing);
	}
	else if (StateMachine->TimeSinceLastFired > 3.0f)
	{
		return CastToUnderlyingType(ECorpusUpperState::Idle);
	}
	else
	{
		return this->GetID();
	}
}

void FCorpusUpperState_Ironsight::OnEnter(int32 StateFromID)
{}

void FCorpusUpperState_Ironsight::OnExit()
{}

int32 FCorpusUpperState_Ironsight::OnCustomEvent(int32 EventID)
{
	switch (static_cast<ECorpusActionEvent>(EventID))
	{
	case ECorpusActionEvent::Reload:
		return CastToUnderlyingType(ECorpusUpperState::Reloading);
	default:
		return this->GetID();
	}
}



int32 FCorpusUpperState_Reloading::GetID()const
{
	return CastToUnderlyingType(ECorpusUpperState::Reloading);
}

int32 FCorpusUpperState_Reloading::OnUpdate(float DeltaTime)
{
	if (Character->GetCurrentWeapon()->IsReloading())
	{
		return this->GetID();
	}
	else
	{
		return CastToUnderlyingType(ECorpusUpperState::Idle);
	}
}

void FCorpusUpperState_Reloading::OnEnter(int32 StateFromID)
{
	Character->GetCurrentWeapon()->BeginReload();
}

void FCorpusUpperState_Reloading::OnExit()
{
	Character->GetCurrentWeapon()->StopReload();
}

int32 FCorpusUpperState_Reloading::OnCustomEvent(int32 EventID)
{
	return this->GetID();
}
