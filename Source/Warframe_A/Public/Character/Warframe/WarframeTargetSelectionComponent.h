
#pragma once

#include "Character/TargetSelectionComponent.h"
#include "WarframeTargetSelectionComponent.generated.h"


UCLASS()
class WARFRAME_A_API UWarframeTargetSelectionComponent : public UTargetSelectionComponent
{
	GENERATED_BODY()

public:
	virtual void UpdateSelectedTarget()override;
};