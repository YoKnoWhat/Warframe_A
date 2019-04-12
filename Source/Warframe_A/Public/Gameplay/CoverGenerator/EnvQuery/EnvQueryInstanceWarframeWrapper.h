// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Runtime/AIModule/Classes/EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvQueryInstanceWarframeWrapper.generated.h"


UCLASS(Blueprintable, BlueprintType)
class WARFRAME_A_API UEnvQueryInstanceWarframeWrapper : public UEnvQueryInstanceBlueprintWrapper
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "AI|EQS")
	TArray<class UCoverPoint*> GetResultsAsCoverPoints();
};
