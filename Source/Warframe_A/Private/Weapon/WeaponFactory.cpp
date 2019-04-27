
#include "Weapon/WeaponFactory.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/WarframeGameInstance.h"
#include "Utility/HelperFunction.h"
#include "Weapon/Primary/BratonPrime.h"
#include "Weapon/Secondary/Staticor.h"

#include "Runtime/Engine/Classes/Engine/World.h"


FWeaponFactory& FWeaponFactory::Instance()
{
	static FWeaponFactory Inst;
	return Inst;
}

AWeaponBase* FWeaponFactory::SpawnWeaponImpl(AActor* Owner, EWeaponID WeaponID, const FTransform& Transform)
{
	AWeaponBase* Weapon;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UClass** Result = ClassOverrides.Find(WeaponID);
	if (Result != nullptr)
	{
		Weapon = Owner->GetWorld()->SpawnActor<AWeaponBase>(*Result, Transform, SpawnParams);
	}
	else
	{
		switch (WeaponID)
		{
		case EWeaponID::BratonPrime:
			Weapon = Owner->GetWorld()->SpawnActor<AWeaponBase>(ABratonPrime::StaticClass(), Transform, SpawnParams);
			break;
		case EWeaponID::Staticor:
			Weapon = Owner->GetWorld()->SpawnActor<AWeaponBase>(AStaticor::StaticClass(), Transform, SpawnParams);
			break;
		case EWeaponID::None:
			return nullptr;
		default:
			Weapon = Owner->GetWorld()->SpawnActor<AWeaponBase>(AWeaponBase::StaticClass(), Transform, SpawnParams);
			break;
		}
	}

	Weapon->Init(WeaponID);
	Weapon->Instigator = Cast<AWarframeCharacter>(Owner);
	return Weapon;
}

void FWeaponFactory::SetOverride(EWeaponID WeaponID, UClass* OverrideClass)
{
	ClassOverrides.Add(WeaponID, OverrideClass);
}

void FWeaponFactory::ClearOverrides()
{
	ClassOverrides.Empty();
}