
#include "Gameplay/PickableObject/PickableOrb.h"
#include "Character/Warframe/Warframe.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"


APickableOrb::APickableOrb(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer)
{}

void APickableOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickableOrb::Init(EPickableObjectID ID)
{
	Super::Init(ID);

	OrbID = ID;

	Value = FMath::RandBool() ? 25.0f : 50.0f;
}

void APickableOrb::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (this->GetWorld()->GetFirstPlayerController()->GetPawn() == OtherActor)
	{
		AWarframe* Warframe = Cast<AWarframe>(OtherActor);

		if (OrbID == EPickableObjectID::HealthOrb)
		{
			Warframe->GainHealth(Value);
		}
		else // if (OrbID == EPickableObjectID::EnergyOrb)
		{
			Warframe->GainEnergy(Value);
		}

		this->Destroy();
	}
}
