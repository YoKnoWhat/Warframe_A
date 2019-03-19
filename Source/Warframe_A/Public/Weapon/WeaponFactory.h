
#pragma once

#include "WarframeCommon.h"


class FWeaponFactory
{
public:
	static FWeaponFactory& Instance();

	AWeaponBase* CreateWeapon(UObject* Outer, EWeaponID WeaponID);

	void SetOverride(EWeaponID WeaponID, UClass* OverrideClass);

protected:
	TMap<EWeaponID, UClass*> OverrideClasses;
};