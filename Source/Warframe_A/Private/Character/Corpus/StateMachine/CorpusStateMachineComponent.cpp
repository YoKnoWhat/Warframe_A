
#include "Character/Corpus/StateMachine/CorpusStateMachineComponent.h"
#include "Character/StateObject.h"


UCorpusStateMachineComponent::UCorpusStateMachineComponent() :
	Super(),
	TimeSinceLastFired(99999.0f)
{}

void UCorpusStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastFired += DeltaTime;
}
