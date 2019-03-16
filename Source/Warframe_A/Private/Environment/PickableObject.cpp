
#include "Environment/PickableObject.h"
#include "Gameplay/WarframeGameInstance.h"

#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Engine/World.h"


APickableObject::APickableObject(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->InitSphereRadius(50.0f);
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

	this->BeamColor = PickableObjectInfo->BeamColor;
	this->StaticMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *PickableObjectInfo->Mesh.ToString()));
}
