
#include "Utility/PhysicsFunction.h"
#include "Runtime/Engine/Classes/Engine/World.h"


namespace WarframeUtil
{
	bool SphereOverlapActors(UObject* WorldContextObject, const FVector& SpherePos, float SphereRadius, const TArray<ECollisionChannel>& ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors)
	{
		TArray<FOverlapResult> Overlaps;

		FCollisionObjectQueryParams ObjectParams;
		for (auto ObjectType : ObjectTypes)
		{
			ObjectParams.AddObjectTypesToQuery(ObjectType);
		}

		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActors(ActorsToIgnore);

		WorldContextObject->GetWorld()->OverlapMultiByObjectType(
			Overlaps,
			SpherePos,
			FQuat::Identity,
			ObjectParams,
			FCollisionShape::MakeSphere(SphereRadius),
			TraceParams
		);

		for (int32 OverlapIdx = 0; OverlapIdx < Overlaps.Num(); ++OverlapIdx)
		{
			FOverlapResult const& O = Overlaps[OverlapIdx];
			if (O.Actor.IsValid())
			{
				if (O.Actor.Get()->IsA(ActorClassFilter))
				{
					OutActors.AddUnique(O.Actor.Get());
				}
			}
		}
	}
}
