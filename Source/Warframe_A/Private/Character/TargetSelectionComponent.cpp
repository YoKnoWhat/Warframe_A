
#include "Character/TargetSelectionComponent.h"


const FHitResult& UTargetSelectionComponent::GetSelectedTarget()const
{
	return SelectedTarget;
}

void UTargetSelectionComponent::UpdateSelectedTarget()
{}
