
#pragma once

#include "Weapon/RoundBase.h"
#include "StaticorRound.generated.h"


UCLASS()
class WARFRAME_A_API AStaticorRound : public ARoundBase
{
	GENERATED_BODY()
		
public:
	virtual void Tick(float DeltaTime)override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

	virtual void Init(AWeaponBase* Weapon)override;

protected:
	bool bIsFullCharged;
};
