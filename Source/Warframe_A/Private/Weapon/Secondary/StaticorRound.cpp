
#include "Weapon/Secondary/StaticorRound.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Utility/PhysicsFunction.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void AStaticorRound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AStaticorRound::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor != this->Instigator)
	{
		float ExplosionRadius;

		if (bIsFullCharged)
		{
			ExplosionRadius = 800.0f;
		}
		else
		{
			ExplosionRadius = 200.0f;
		}

		TArray<ECollisionChannel> ObjectTypeArray = { ECollisionChannel::ECC_Pawn };
		TArray<AActor*> ActorsToIgnore = { this->Instigator };
		TArray<AActor*> OverlapActors;

		WarframeUtil::SphereOverlapActors(this, GetActorLocation(), ExplosionRadius, ObjectTypeArray, AWarframeCharacter::StaticClass(), ActorsToIgnore, OverlapActors);

		for (AActor* Actor : OverlapActors)
		{
			this->OnHit(Actor, Actor->GetActorLocation());
		}

		UParticleSystem* ParticleSystem = FWarframeConfigSingleton::Instance().FindResource<UParticleSystem>("P_StaticorExplosion");

		UParticleSystemComponent* UGameplayStatics::SpawnEmitterAtLocation(this, ParticleSystem, GetActorLocation());

		this->Destroy();
	}
}

void AStaticorRound::Init(AWeaponBase* Weapon)
{
	Super::Init(Weapon);
}
