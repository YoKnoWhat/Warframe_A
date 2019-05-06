
#include "Weapon/RoundInstant.h"
#include "Character/WarframeCharacter.h"

#include "Runtime/Engine/Classes/Engine/World.h"


ARoundInstant::ARoundInstant(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{}

void ARoundInstant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;

	if (this->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		this->GetActorLocation(),
		this->GetActorLocation() + this->GetActorForwardVector() * 10000.0f,
		ECollisionChannel::ECC_Weapon,
		QueryParams) == true)
	{
		this->OnHit(HitResult.Actor.Get(), HitResult.ImpactPoint, HitResult.BoneName);
	}

	this->Destroy();
}
