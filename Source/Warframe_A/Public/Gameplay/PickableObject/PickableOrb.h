
#pragma once

#include "Gameplay/PickableObject/PickableObject.h"
#include "PickableOrb.generated.h"


UCLASS()
class WARFRAME_A_API APickableOrb : public APickableObject
{
	GENERATED_BODY()

public:
	APickableOrb(const FObjectInitializer &ObjectInitializer);

	virtual void Tick(float DeltaTime)override;

	virtual void Init(EPickableObjectID ID)override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

protected:
	EPickableObjectID OrbID;

	float Value;
};
