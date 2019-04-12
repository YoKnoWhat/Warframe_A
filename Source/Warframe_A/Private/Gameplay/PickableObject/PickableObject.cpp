
#include "Gameplay/PickableObject/PickableObject.h"
#include "Gameplay/WarframeGameInstance.h"
#include "Gameplay/WarframeConfigSingleton.h"

#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


APickableObject::APickableObject(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "SphereCollision");
	SetRootComponent(SphereCollision);
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->InitSphereRadius(50.0f);

	StaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "StaticMesh");
	StaticMesh->SetupAttachment(SphereCollision);

	ParticleSystem = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, "ParticleSystem");
	ParticleSystem->SetupAttachment(SphereCollision);
	ParticleSystem->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
}

void APickableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FLinearColor::Red;
}

void APickableObject::Init(EPickableObjectID ID)
{
	UWarframeGameInstance* GameInstance = Cast<UWarframeGameInstance>(this->GetWorld()->GetGameInstance());

	const FPickableObjectInfo* PickableObjectInfo = GameInstance->GetPickableObjectInfo(ID);

	this->StaticMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *PickableObjectInfo->Mesh.ToString()));
	this->ParticleSystem->SetTemplate(FWarframeConfigSingleton::Instance().FindResource<UParticleSystem>("PS_AmmoBeam"));
	this->ParticleSystem->SetColorParameter("Color", PickableObjectInfo->BeamColor);
}
