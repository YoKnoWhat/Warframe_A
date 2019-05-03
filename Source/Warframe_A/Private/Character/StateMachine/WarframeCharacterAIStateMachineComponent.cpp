
#include "Character/StateMachine/WarframeCharacterAIStateMachineComponent.h"


FWarframeCharacterAILowerState_Idle						UWarframeCharacterAIStateMachineComponent::LowerIdleState;
FWarframeCharacterAILowerState_AtCoverStanding			UWarframeCharacterAIStateMachineComponent::LowerAtCoverStandingState;
FWarframeCharacterAILowerState_AtCoverCrouching			UWarframeCharacterAIStateMachineComponent::LowerAtCoverCrouchingState;
FWarframeCharacterAILowerState_AtCoverFiringStanding	UWarframeCharacterAIStateMachineComponent::LowerAtCoverFiringStandingState;
FWarframeCharacterAILowerState_AtCoverFiringCrouching	UWarframeCharacterAIStateMachineComponent::LowerAtCoverFiringCrouchingState;

UWarframeCharacterAIStateMachineComponent::UWarframeCharacterAIStateMachineComponent() :
	Super()
{}

void UWarframeCharacterAIStateMachineComponent::Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer)
{
	LayerInitializer.SetLayer(CastToUnderlyingType(EWarframeCharacterAIStateLayer::Lower), new FWarframeCharacterAIStateMachineLayer_Lower);

	Super::Init(InCharacter, LayerInitializer);

	LowerLayer = static_cast<FWarframeCharacterAIStateMachineLayer_Lower*>(LayerInitializer.GetLayer(CastToUnderlyingType(EWarframeCharacterAIStateLayer::Lower)));
	LowerLayer->IdleState = &LowerIdleState;
	LowerLayer->AtCoverStandingState = &LowerAtCoverStandingState;
	LowerLayer->AtCoverCrouchingState = &LowerAtCoverCrouchingState;
	LowerLayer->AtCoverFiringStandingState = &LowerAtCoverFiringStandingState;
	LowerLayer->AtCoverFiringCrouchingState = &LowerAtCoverFiringCrouchingState;
}

void UWarframeCharacterAIStateMachineComponent::ReInit()
{
	Super::ReInit();
}

void UWarframeCharacterAIStateMachineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
