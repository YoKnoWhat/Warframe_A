
#pragma once

#include "Weapon/WeaponBase.h"
#include "Staticor.generated.h"


UCLASS()
class WARFRAME_A_API AStaticor : public AWeaponBase
{
	GENERATED_BODY()

public:
	AStaticor(const FObjectInitializer& ObjectInitializer);

	virtual UClass* GetRoundClass_Implementation()const override;

	bool IsFullCharged()const;
};
