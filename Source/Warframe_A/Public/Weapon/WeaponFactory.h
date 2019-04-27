
#pragma once

#include "WarframeCommon.h"


class AActor;

class FWeaponFactory
{
public:
	static FWeaponFactory& Instance();

	template<class T>
	T* SpawnWeapon(AActor* Owner, EWeaponID WeaponID, const FTransform& Transform)
	{
		return Cast<T>(this->SpawnWeaponImpl(Owner, WeaponID, Transform));
	}
	
	void SetOverride(EWeaponID WeaponID, UClass* OverrideClass);

	void ClearOverrides();

protected:
	AWeaponBase* SpawnWeaponImpl(AActor* Owner, EWeaponID WeaponID, const FTransform& Transform);

protected:
	TMap<EWeaponID, UClass*> ClassOverrides;
};