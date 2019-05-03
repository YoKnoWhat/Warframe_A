
#include "Character/Warframe/WarframeTargetSelectionComponent.h"
#include "Character/WarframeCharacter.h"

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"


void UWarframeTargetSelectionComponent::UpdateSelectedTarget()
{
	AActor* OwnerCharacter = Cast<AController>(this->GetOwner())->GetPawn();

	UCameraComponent *CameraComponent = Cast<UCameraComponent>(OwnerCharacter->GetComponentByClass(UCameraComponent::StaticClass()));

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

	FCollisionQueryParams RV_TraceParams;
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.AddIgnoredActor(OwnerCharacter);

	AWarframeCharacter* PrevTarget = Cast<AWarframeCharacter>(SelectedTarget.Actor.Get());

	if (GetWorld()->LineTraceSingleByObjectType(
		SelectedTarget,
		CameraComponent->GetComponentLocation(),
		CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 10000.0f,
		ObjectQueryParams,
		RV_TraceParams) == false)
	{
		SelectedTarget.ImpactPoint = SelectedTarget.TraceEnd;
	}

	AWarframeCharacter* NewTarget = Cast<AWarframeCharacter>(SelectedTarget.Actor.Get());

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
