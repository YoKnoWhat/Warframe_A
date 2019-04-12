
#include "Character/Grineer/StateMachine/GrineerStateMachineComponent.h"
#include "Character/StateObject.h"


UGrineerStateMachineComponent::UGrineerStateMachineComponent() :
	Super(),
	TimeSinceLastFired(99999.0f)
{}

void UGrineerStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastFired += DeltaTime;
}
