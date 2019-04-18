
#include "Character/StateMachineLayerInitializer.h"
#include "Character/StateMachineComponent.h"


FStateMachineLayer* FStateMachineLayerInitializer::GetLayer(uint32 LayerIndex)
{
	return LayerMap[LayerIndex];
}
