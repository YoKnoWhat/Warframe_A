
#pragma once

#include "Character/TargetSelectionComponent.h"
#include "AITargetSelectionComponent.generated.h"


UCLASS()
class WARFRAME_A_API UAITargetSelectionComponent : public UTargetSelectionComponent
{
	GENERATED_BODY()

public:
	// todo: for now.
	UFUNCTION(BlueprintCallable)
	void SetSelectedTarget(const FHitResult& Target)
	{
		SelectedTarget = Target;
	}

	virtual void UpdateSelectedTarget()override;
};