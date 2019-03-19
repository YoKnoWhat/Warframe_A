
#include "Weapon/WeaponFactory.h"
#include "Weapon/Secondary/Staticor.h"

#include "Runtime/Engine/Classes/Engine/World.h"


FWeaponFactory& FWeaponFactory::Instance()
{
	static FWeaponFactory Inst;
	return Inst;
}

AWeaponBase* FWeaponFactory::CreateWeapon(UObject* Outer, EWeaponID WeaponID)
{
	UClass** Result = OverrideClasses.Find(WeaponID);
	if (Result != nullptr)
	{
		return NewObject<AWeaponBase>(Outer, *Result);
	}
	else
	{
		switch (WeaponID)
		{
		case EWeaponID::Staticor:
			return NewObject<AStaticor>(Outer);
		default:
			return NewObject<AWeaponBase>(Outer);
		}
	}
}

void FWeaponFactory::SetOverride(EWeaponID WeaponID, UClass* OverrideClass)
{
	OverrideClasses.Add(WeaponID, OverrideClass);
}