
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Character/WarframeCharacter.h"
#include "Character/StateObject.h"

#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"


UWarframeStateMachineComponent::UWarframeStateMachineComponent() :
	Super(),
	GlideTimer(3.0f),
	TimeSinceLastFired(99999.0f)
{}

void UWarframeStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastFired += DeltaTime;

	if (Cast<AWarframeCharacter>(this->GetOwner())->GetMovementComponent()->IsFalling() == false)
	{
		GlideTimer = FMath::Min(GlideTimer + DeltaTime, 3.0f);
	}
}
