
#include "Environment/PickableObject.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"


APickableObject::APickableObject(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
}

void APickableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FLinearColor::Red;
}
