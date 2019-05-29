// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "InputCoreTypes.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateAtCoverDirection.generated.h"


/**
 * Default Focus service node.
 * A service node that automatically sets the AI controller's focus when it becomes active.
 */
UCLASS()
class WARFRAME_A_API UBTService_UpdateAtCoverDirection : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdateAtCoverDirection(const FObjectInitializer& ObjectInitializer);

protected:
	virtual FString GetStaticDescription()const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

protected:
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetKey;

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector CoverPointKey;

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector AtCoverDirectionKey;

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector NeedToChangeAtCoverDirectionKey;

protected:

};
