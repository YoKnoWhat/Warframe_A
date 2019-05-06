
#include "Character/Warframe/WarframeTargetSelectionComponent.h"
#include "Character/WarframeCharacter.h"

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/SceneView.h"


void UWarframeTargetSelectionComponent::UpdateSelectedTarget()
{
	ULocalPlayer* LocalPlayer = Cast<APlayerController>(this->GetOwner())->GetLocalPlayer();
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FVector2D ScreenCenter;
		LocalPlayer->ViewportClient->GetViewportSize(ScreenCenter);
		ScreenCenter *= 0.5f;

		// Get the projection data.
		FSceneViewProjectionData ProjectionData;
		if (LocalPlayer->GetProjectionData(LocalPlayer->ViewportClient->Viewport, eSSP_FULL, /*out*/ ProjectionData))
		{
			FVector WorldPosition;
			FVector WorldDirection;

			FMatrix const InvViewProjMatrix = ProjectionData.ComputeViewProjectionMatrix().InverseFast();
			FSceneView::DeprojectScreenToWorld(ScreenCenter, ProjectionData.GetConstrainedViewRect(), InvViewProjMatrix, /*out*/ WorldPosition, /*out*/ WorldDirection);

			// Do line trace.
			{
				AActor* OwnerCharacter = Cast<AController>(this->GetOwner())->GetPawn();

				FCollisionQueryParams RV_TraceParams;
				RV_TraceParams.bTraceComplex = false;
				RV_TraceParams.AddIgnoredActor(OwnerCharacter);

				AWarframeCharacter* PrevTarget = Cast<AWarframeCharacter>(SelectedTarget.Actor.Get());

				if (GetWorld()->LineTraceSingleByChannel(
					SelectedTarget,
					WorldPosition,
					WorldPosition + WorldDirection * 10000.0f,
					ECollisionChannel::ECC_Weapon,
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
		}
	}
}
