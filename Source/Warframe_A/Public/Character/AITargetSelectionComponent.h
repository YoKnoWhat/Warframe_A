
#pragma once

#include "Character/TargetSelectionComponent.h"
#include "AITargetSelectionComponent.generated.h"


UCLASS()
class WARFRAME_A_API UAITargetSelectionComponent : public UTargetSelectionComponent
{
	GENERATED_BODY()

public:
	virtual void UpdateSelectedTarget()override;
};