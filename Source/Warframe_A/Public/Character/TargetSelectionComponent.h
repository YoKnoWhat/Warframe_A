
#pragma once

#include "Components/ActorComponent.h"
#include "TargetSelectionComponent.generated.h"


UCLASS()
class WARFRAME_A_API UTargetSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	const FHitResult& GetSelectedTarget()const;

	virtual void UpdateSelectedTarget();

protected:
	FHitResult SelectedTarget;
};