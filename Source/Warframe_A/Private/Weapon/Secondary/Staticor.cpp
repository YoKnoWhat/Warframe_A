
#include "Weapon/Secondary/Staticor.h"
#include "Weapon/TriggerModifier.h"


void AStaticor::Init(EWeaponID WeaponID, uint32 Level_)
{
	Super::Init(WeaponID, Level_);

	// this->MaxChargedRounds = 4;
}

int32 AStaticor::GetChargeCount()const
{
	return dynamic_cast<FTriggerModifier_Charge*>(FireModeArray[CurrentFireMode].TriggerModifier)->GetCurrentChargeRounds();
}
