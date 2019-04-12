
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

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	this->GetActorForwardVector();

	if (this->GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		this->GetActorLocation(),
		this->GetActorLocation() + this->GetActorForwardVector() * 10000.0f,
		ObjectQueryParams,
		QueryParams) == true)
	{
		if (Cast<AWarframeCharacter>(HitResult.Actor.Get()) != nullptr)
		{
			this->OnHit(HitResult.Actor.Get(), HitResult.ImpactPoint);
		}
	}

	this->Destroy();
}
