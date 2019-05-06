
#include "Weapon/Secondary/StaticorRound.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Utility/PhysicsFunction.h"
#include "Weapon/Secondary/Staticor.h"

#include "Runtime/Engine/Classes/Components/ShapeComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsAsset.h"


AStaticorRound::AStaticorRound(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	UProjectileMovementComponent* ProjectileMovement = Cast<UProjectileMovementComponent>(MovementComponent);
	ProjectileMovement->InitialSpeed = 5000.0f;
	ProjectileMovement->MaxSpeed = 5000.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AStaticorRound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStaticorRound::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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

		AWarframeCharacter* OtherCharacter = Cast<AWarframeCharacter>(OtherActor);
		if (OtherCharacter != nullptr)
		{
			USkeletalBodySetup* SkeletalBodySetup = OtherCharacter->GetMesh()->GetPhysicsAsset()->SkeletalBodySetups[OtherBodyIndex];

			this->OnHit(OtherCharacter, SweepResult.ImpactPoint, SkeletalBodySetup->BoneName);
		}

		TArray<ECollisionChannel> ObjectTypeArray;
		ObjectTypeArray.Add(ECollisionChannel::ECC_Pawn);
		ObjectTypeArray.Add(ECollisionChannel::ECC_Destructible);
		TArray<AActor*> ActorsToIgnore = { this->Instigator };
		TArray<AActor*> OverlapActors;

		WarframeUtil::SphereOverlapActors(this, GetActorLocation(), ExplosionRadius, ObjectTypeArray, AWarframeCharacter::StaticClass(), ActorsToIgnore, OverlapActors);

		for (AActor* Actor : OverlapActors)
		{
			this->OnHit(Actor, Actor->GetActorLocation(), "None");
		}

		UParticleSystem* ParticleSystem = FWarframeConfigSingleton::Instance().FindResource<UParticleSystem>("PS_StaticorExplosion");

		UGameplayStatics::SpawnEmitterAtLocation(this, ParticleSystem, GetActorLocation());

		this->Destroy();
	}
}

void AStaticorRound::Init(AWeaponBase* Weapon)
{
	Super::Init(Weapon);

	this->bIsFullCharged = Cast<AStaticor>(Weapon)->IsFullCharged();
}
