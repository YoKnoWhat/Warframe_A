
#include "Character/Warframe/WarframeTargetSelectionComponent.h"
#include "Character/WarframeCharacter.h"

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"


void UWarframeTargetSelectionComponent::UpdateSelectedTarget()
{
	AActor* OwnerCharacter = Cast<AController>(this->GetOwner())->GetPawn();

	UCameraComponent *CameraComponent = Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));

	FCollisionQueryParams RV_TraceParams;
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.AddIgnoredActor(OwnerCharacter);

	AWarframeCharacter* PrevTarget = Cast<AWarframeCharacter>(this->SelectedTarget.Actor.Get());

	if (GetWorld()->LineTraceSingleByChannel(
		this->SelectedTarget,
		CameraComponent->GetComponentLocation(),
		CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 10000.0f,
		ECC_Pawn,
		RV_TraceParams) == false)
	{
		this->SelectedTarget.ImpactPoint = this->SelectedTarget.TraceEnd;
	}

	AWarframeCharacter* NewTarget = Cast<AWarframeCharacter>(this->SelectedTarget.Actor.Get());

	if (PrevTarget != NewTarget)
	{
		if (PrevTarget != nullptr)
		{
			PrevTarget->OnUnselected();
		}
		if (NewTarget != nullptr)
		{
			NewTarget->OnSelected();
		}
	}
}
