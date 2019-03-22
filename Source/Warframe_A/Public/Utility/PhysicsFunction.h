
#pragma once

#include "Runtime/CoreUObject/Public/UObject/Object.h"
#include "Runtime/Engine/Classes/Engine/World.h"


namespace WarframeUtil
{
	bool SphereOverlapActors(UObject* WorldContextObject, const FVector& SpherePos, float SphereRadius, const TArray<enum ECollisionChannel>& ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors);
}
