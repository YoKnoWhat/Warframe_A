
#include "Weapon/RoundInstant.h"
#include "Character/WarframeCharacter.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"


ARoundInstant::ARoundInstant(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{}

void ARoundInstant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// todo: For now.
	// UKismetSystemLibrary::DrawDebugLine(this, this->GetActorLocation(), this->GetActorLocation() + this->GetActorForwardVector() * 10000.0f, FLinearColor(0.0f, 1.0f, 0.0f, 1.0f), 0.16f);

	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.AddIgnoredActor(this->GetOwner());
	QueryParams.AddIgnoredActor(this->Instigator);

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
