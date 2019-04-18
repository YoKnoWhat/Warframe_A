
#include "Character/StateMachine/WarframeCharacterStateMachineComponent.h"


FName UWarframeCharacterStateMachineComponent::LowerLayerName("Lower");
FName UWarframeCharacterStateMachineComponent::UpperLayerName("Upper");

FWarframeCharacterLowerState_Crouching	UWarframeCharacterStateMachineComponent::LowerCrouchingState;
FWarframeCharacterLowerState_Falling	UWarframeCharacterStateMachineComponent::LowerFallingState;
FWarframeCharacterLowerState_Idle		UWarframeCharacterStateMachineComponent::LowerIdleState;
FWarframeCharacterLowerState_Jumping	UWarframeCharacterStateMachineComponent::LowerJumpingState;
FWarframeCharacterLowerState_Sprinting	UWarframeCharacterStateMachineComponent::LowerSprintingState;

FWarframeCharacterUpperState_Firing				UWarframeCharacterStateMachineComponent::UpperFiringState;
FWarframeCharacterUpperState_Idle				UWarframeCharacterStateMachineComponent::UpperIdleState;
FWarframeCharacterUpperState_Ironsight			UWarframeCharacterStateMachineComponent::UpperIronsightState;
FWarframeCharacterUpperState_Reloading			UWarframeCharacterStateMachineComponent::UpperReloadingState;
FWarframeCharacterUpperState_WeaponSwitching	UWarframeCharacterStateMachineComponent::UpperWeaponSwitchingState;

UWarframeCharacterStateMachineComponent::UWarframeCharacterStateMachineComponent() :
	Super(),
	TimeSinceLastFired(99999.0f)
{}

void UWarframeCharacterStateMachineComponent::Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer)
{
	LayerInitializer.SetLayer(CastToUnderlyingType(EWarframeCharacterStateLayer::Lower), new FWarframeCharacterStateMachineLayer_Lower)
		.SetLayer(CastToUnderlyingType(EWarframeCharacterStateLayer::Upper), new FWarframeCharacterStateMachineLayer_Upper);

	Super::Init(InCharacter, LayerInitializer);

	LowerLayer = static_cast<FWarframeCharacterStateMachineLayer_Lower*>(LayerInitializer.GetLayer(CastToUnderlyingType(EWarframeCharacterStateLayer::Lower)));
	LowerLayer->CrouchingState = &LowerCrouchingState;
	LowerLayer->FallingState = &LowerFallingState;
	LowerLayer->IdleState = &LowerIdleState;
	LowerLayer->JumpingState = &LowerJumpingState;
	LowerLayer->SprintingState = &LowerSprintingState;

	UpperLayer = static_cast<FWarframeCharacterStateMachineLayer_Upper*>(LayerInitializer.GetLayer(CastToUnderlyingType(EWarframeCharacterStateLayer::Upper)));
	UpperLayer->FiringState = &UpperFiringState;
	UpperLayer->IdleState = &UpperIdleState;
	UpperLayer->IronsightState = &UpperIronsightState;
	UpperLayer->ReloadingState = &UpperReloadingState;
	UpperLayer->WeaponSwitchingState = &UpperWeaponSwitchingState;
}

void UWarframeCharacterStateMachineComponent::ReInit()
{
	Super::ReInit();

	LowerLayer->IsEnabled = true;
	LowerLayer->CurrentState = LowerLayer->IdleState;
	LowerLayer->CurrentState->OnEnter(this, nullptr);

	UpperLayer->IsEnabled = true;
	UpperLayer->CurrentState = UpperLayer->IdleState;
	UpperLayer->CurrentState->OnEnter(this, nullptr);
}

void UWarframeCharacterStateMachineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastFired += DeltaTime;
}
