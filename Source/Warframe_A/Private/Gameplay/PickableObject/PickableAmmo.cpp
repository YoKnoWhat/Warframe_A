
#include "Gameplay/PickableObject/PickableAmmo.h"


APickableAmmo::APickableAmmo(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer)
{}

void APickableAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickableAmmo::Init(EPickableObjectID ID)
{
	Super::Init(ID);
}

void APickableAmmo::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}
