
#pragma once

#include "Weapon/WeaponBase.h"
#include "Staticor.generated.h"


UCLASS()
class WARFRAME_A_API AStaticor : public AWeaponBase
{
	GENERATED_BODY()

public:
	virtual void Init(EWeaponID WeaponID, uint32 Level/*Polarities, Mods*/)override;

	UFUNCTION(BlueprintCallable)
	int32 GetChargeCount()const;

protected:
	UPROPERTY(BlueprintReadOnly)
	float ProjectileFlightSpeed = 5000.0f;
};
