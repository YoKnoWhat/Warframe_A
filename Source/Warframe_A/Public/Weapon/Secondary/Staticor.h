
#pragma once

#include "Weapon/WeaponBase.h"
#include "Staticor.generated.h"


UCLASS()
class WARFRAME_A_API AStaticor : public AWeaponBase
{
	GENERATED_BODY()

public:
	AStaticor(const FObjectInitializer& ObjectInitializer);

	virtual void Init(EWeaponID WeaponID, uint32 Level/*Polarities, Mods*/)override;

	virtual UClass* GetRoundClass_Implementation()const override;

	bool IsFullCharged()const;
};
