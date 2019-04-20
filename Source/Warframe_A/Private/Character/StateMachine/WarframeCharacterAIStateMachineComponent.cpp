
#include "Character/StateMachine/WarframeCharacterAIStateMachineComponent.h"


FWarframeCharacterAILowerState_AtCover	UWarframeCharacterAIStateMachineComponent::LowerAtCoverState;
FWarframeCharacterAILowerState_Idle		UWarframeCharacterAIStateMachineComponent::LowerIdleState;

UWarframeCharacterAIStateMachineComponent::UWarframeCharacterAIStateMachineComponent() :
	Super()
{}

void UWarframeCharacterAIStateMachineComponent::Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer)
{
	LayerInitializer.SetLayer(CastToUnderlyingType(EWarframeCharacterAIStateLayer::Lower), new FWarframeCharacterAIStateMachineLayer_Lower);

	Super::Init(InCharacter, LayerInitializer);

	LowerLayer = static_cast<FWarframeCharacterAIStateMachineLayer_Lower*>(LayerInitializer.GetLayer(CastToUnderlyingType(EWarframeCharacterAIStateLayer::Lower)));
	LowerLayer->IdleState = &LowerIdleState;
	LowerLayer->AtCoverState = &LowerAtCoverState;
}

void UWarframeCharacterAIStateMachineComponent::ReInit()
{
	Super::ReInit();
}

void UWarframeCharacterAIStateMachineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
