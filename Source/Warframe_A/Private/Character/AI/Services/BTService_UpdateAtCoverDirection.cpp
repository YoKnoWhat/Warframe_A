// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Character/AI/Services/BTService_UpdateAtCoverDirection.h"
#include "Character/StateMachine/WarframeCharacterAIStateMachineComponent.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/CoverGenerator/CoverPoint.h"

#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


UBTService_UpdateAtCoverDirection::UBTService_UpdateAtCoverDirection(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	NodeName = "Update At Cover Direction";
}

FString UBTService_UpdateAtCoverDirection::GetStaticDescription()const
{
	return FString::Printf(TEXT("%s\nResult Blackboard key: %s, %s"),
		*Super::GetStaticDescription(),
		*AtCoverDirectionKey.SelectedKeyName.ToString(),
		*NeedToChangeAtCoverDirectionKey.SelectedKeyName.ToString()
	);
}

void UBTService_UpdateAtCoverDirection::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	AActor* Target = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));
	UCoverPoint* CoverPoint = Cast<UCoverPoint>(BlackboardComponent->GetValueAsObject(CoverPointKey.SelectedKeyName));

	FVector CoverPointRight = FVector::CrossProduct(FVector::UpVector, CoverPoint->DirectionToWall());
	FVector CoverToTarget = (Target->GetActorLocation() - CoverPoint->Location).GetUnsafeNormal();

	int32 OldAtCoverDirection = BlackboardComponent->GetValueAsInt(AtCoverDirectionKey.SelectedKeyName);
	int32 NewAtCoverDirection;

	// Target is at cover's right direction.
	if (FVector::DotProduct(CoverPointRight, CoverToTarget) > 0.0f)
	{
		NewAtCoverDirection = +1;
	}
	else 
	{
		NewAtCoverDirection = -1;
	}

	if (OldAtCoverDirection != NewAtCoverDirection)
	{
		BlackboardComponent->SetValueAsInt(AtCoverDirectionKey.SelectedKeyName, NewAtCoverDirection);
		BlackboardComponent->SetValueAsBool(NeedToChangeAtCoverDirectionKey.SelectedKeyName, true);
	}
}
