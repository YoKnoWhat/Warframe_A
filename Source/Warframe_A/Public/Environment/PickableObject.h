
#pragma once

#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "PickableObject.generated.h"


UCLASS()
class WARFRAME_A_API APickableObject : public AActor
{
	GENERATED_BODY()

public:
	APickableObject(const FObjectInitializer &ObjectInitializer);

	virtual void Tick(float DeltaTime)override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetStaticMesh()
	{
		return StaticMesh;
	}

protected:
	UStaticMeshComponent* StaticMesh;

	FLinearColor BeamColor;


};