
#pragma once

#include "Weapon/RoundBase.h"
#include "StaticorRound.generated.h"


UCLASS()
class WARFRAME_A_API AStaticorRound : public ARoundBase
{
	GENERATED_BODY()
		
public:
	AStaticorRound(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime)override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

	virtual void Init(AWeaponBase* Weapon)override;

protected:
	bool bIsFullCharged;
};
