
#pragma once

#include "Environment/PickableObject.h"
#include "PickableAmmo.generated.h"


UCLASS()
class WARFRAME_A_API APickableAmmo : public APickableObject
{
	GENERATED_BODY()

public:
	APickableAmmo(const FObjectInitializer &ObjectInitializer);

	virtual void Tick(float DeltaTime)override;

	virtual void Init(EPickableObjectID ID)override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

protected:
};
