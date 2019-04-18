
#include "Character/Corpus/StateMachine/CorpusStateMachineComponent.h"
#include "Character/StateObject.h"


UCorpusStateMachineComponent::UCorpusStateMachineComponent() :
	Super()
{}

void UCorpusStateMachineComponent::Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer)
{
	Super::Init(InCharacter, LayerInitializer);
}

void UCorpusStateMachineComponent::ReInit()
{
	Super::ReInit();
}

void UCorpusStateMachineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastFired += DeltaTime;
}
