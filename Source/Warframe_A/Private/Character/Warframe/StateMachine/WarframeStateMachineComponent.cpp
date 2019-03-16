
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Character/StateObject.h"


UWarframeStateMachineComponent::UWarframeStateMachineComponent() :
	Super(),
	GlideTimer(3.0f),
	TimeSinceLastFired(99999.0f)
{}

void UWarframeStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastFired += DeltaTime;

	if (this->GetCurrentState(CastToUnderlyingType(EWarframeStateLayer::Lower))->GetID() != CastToUnderlyingType(EWarframeLowerState::AimGliding))
	{
		GlideTimer = FMath::Min(GlideTimer + DeltaTime, 3.0f);
	}
}
