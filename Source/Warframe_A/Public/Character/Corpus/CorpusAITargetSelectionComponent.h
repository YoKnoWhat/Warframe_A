
#pragma once

#include "Character/AITargetSelectionComponent.h"
#include "CorpusAITargetSelectionComponent.generated.h"


UCLASS()
class WARFRAME_A_API UCorpusAITargetSelectionComponent : public UAITargetSelectionComponent
{
	GENERATED_BODY()

public:
	// todo: for now.
	void SetSelectedTarget(const FHitResult& Target)
	{
		SelectedTarget = Target;
	}

	virtual void UpdateSelectedTarget()override;
};