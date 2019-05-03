// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/RoundBase.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Weapon/WeaponBase.h"

#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"


// Sets default values
ARoundBase::ARoundBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "Root"));

	/** Shape component creation. */
	ShapeComponent = Cast<UShapeComponent>(ObjectInitializer.CreateDefaultSubobject(this, "Shape", UShapeComponent::StaticClass(), UCapsuleComponent::StaticClass(), true, false, false));
	ShapeComponent->SetCollisionObjectType(ECollisionChannel::ECC_Round);
	ShapeComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Weapon, ECollisionResponse::ECR_Ignore);
	ShapeComponent->SetupAttachment(this->RootComponent);
	ShapeComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	ShapeComponent->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	/** Mesh component creation. */
	MeshComponent = Cast<UMeshComponent>(ObjectInitializer.CreateDefaultSubobject(this, "Mesh", UMeshComponent::StaticClass(), UStaticMeshComponent::StaticClass(), true, false, false));
	MeshComponent->SetupAttachment(ShapeComponent);

	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(MeshComponent);
	if (StaticMeshComponent != nullptr)
	{
		UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(ShapeComponent);
		if (CapsuleComponent != nullptr)
		{
			StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleComponent->GetUnscaledCapsuleHalfHeight()));
		}

		StaticMeshComponent->SetStaticMesh(FWarframeConfigSingleton::Instance().FindResource<UStaticMesh>("SM_NarrowCapsule"));
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}

	/** Movement component creation. */
	MovementComponent = Cast<UMovementComponent>(ObjectInitializer.CreateDefaultSubobject(this, "Movement", UMovementComponent::StaticClass(), UProjectileMovementComponent::StaticClass(), true, false, false));

	UProjectileMovementComponent* ProjectileMovement = Cast<UProjectileMovementComponent>(MovementComponent);
	if (ProjectileMovement != nullptr)
	{
		ProjectileMovement->InitialSpeed = 3500.0f;
		ProjectileMovement->MaxSpeed = 3500.0f;
		ProjectileMovement->ProjectileGravityScale = 0.05f;
	}
}

// Called when the game starts or when spawned
void ARoundBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARoundBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = this->GetActorLocation();

	FliedDistance += (NewLocation - LastLocation).SizeSquared();

	LastLocation = NewLocation;
}

void ARoundBase::InitBP(AWeaponBase* Weapon)
{
	this->Init(Weapon);
}

void ARoundBase::Init(AWeaponBase* Weapon)
{
	// Init basic attributes.
	this->CriticalTier = Weapon->GetCriticalTier();
	this->CriticalChance = Weapon->GetCriticalChance() - 1.0f * CriticalTier;
	this->CriticalMultiplier = Weapon->GetCriticalMultiplier();
	this->FalloffStart = Weapon->GetFalloffStart();
	this->FalloffEnd = Weapon->GetFalloffEnd();
	this->FalloffDamage = Weapon->GetFalloffDamage();
	this->PunchThrough = Weapon->GetPunchThrough();
	this->BaseDamage = Weapon->GetBaseDamage();
	this->DamageArray = Weapon->GetDamageArray();
	this->BleedMultiplier = Weapon->GetBleedMultiplier();
	this->HeatModMultiplier = Weapon->GetHeatModMultiplier();
	this->ColdModMultiplier = Weapon->GetColdModMultiplier();
	this->ElectricityModMultiplier = Weapon->GetElectricityModMultiplier();
	this->ToxinModMultiplier = Weapon->GetToxinModMultiplier();

	// Status effect roll.
	if (FMath::FRandRange(0.0f, 1.0f) < Weapon->GetStatusChance())
	{
		this->StatusEffect = Weapon->GetStatusEffect(FMath::FRandRange(0.0f, 1.0f));
		switch (StatusEffect)
		{
		case EDamageType::Slash:
			this->StatusDamageMultiplier = this->BleedMultiplier;
			break;
		case EDamageType::Heat:
			this->StatusDamageMultiplier = this->HeatModMultiplier;
			break;
		case EDamageType::Electricity:
			this->StatusDamageMultiplier = this->ElectricityModMultiplier;
			break;
		case EDamageType::Toxin:
			this->StatusDamageMultiplier = this->ToxinModMultiplier;
			break;
		case EDamageType::Gas:
			this->StatusDamageMultiplier = this->ToxinModMultiplier;
			break;
		default:
			this->StatusDamageMultiplier = 1.0f;
			break;
		}
	}
	else
	{
		this->StatusEffect = EDamageType::None;
	}
	this->DamageScalar = 1.0f;

	// Miscellaneous attributes.
	this->LastLocation = this->GetActorLocation();
	this->FliedDistance = 0.0f;
}

void ARoundBase::OnHit(AActor *Target, FVector HitLocation)
{
	AWarframeCharacter* WarframeCharacter = Cast<AWarframeCharacter>(Target);

	if (WarframeCharacter != nullptr)
	{
		// Apply falloff scalar.
		if (FalloffEnd > 0.0f)
		{
			float FalloffScalar = (FalloffEnd - FliedDistance) / (FalloffEnd - FalloffStart);

			this->DamageScalar *= FMath::Clamp(FalloffScalar, this->FalloffDamage, 1.0f);
		}

		//// todo: Apply punch through scalar.
		//if (PunchThrough > 1.0f)
		//{
		//	PunchThrough -= 1.0f;
		//}
		//else // if (PunchThrough <= 1.0f)
		//{
		//	this->DamageScalar *= PunchThrough;
		//	PunchThrough = 0.0f;
		//}

		// Critical hit roll.
		uint32 TempCriticalTier = this->CriticalTier;

		if (FMath::FRandRange(0.0f, 1.0f) < this->CriticalChance)
		{
			++TempCriticalTier;
		}

		DamageScalar *= (1.0 + TempCriticalTier * (CriticalMultiplier - 1.0f));

		WarframeCharacter->ApplyDamage(Instigator, HitLocation, StatusEffect, DamageArray, DamageScalar, TempCriticalTier);

		WarframeCharacter->ApplyStatusEffect(Instigator, HitLocation, StatusEffect, BaseDamage, StatusDamageMultiplier);
	}
	else
	{
		// todo: PunchThrough
	}
}
