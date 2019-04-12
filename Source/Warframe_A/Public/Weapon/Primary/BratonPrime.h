
#pragma once

#include "Weapon/WeaponBase.h"
#include "BratonPrime.generated.h"


UCLASS()
class WARFRAME_A_API ABratonPrime : public AWeaponBase
{
	GENERATED_BODY()

public:
	ABratonPrime(const FObjectInitializer& ObjectInitializer);

	virtual UClass* GetRoundClass_Implementation()const override;
};
