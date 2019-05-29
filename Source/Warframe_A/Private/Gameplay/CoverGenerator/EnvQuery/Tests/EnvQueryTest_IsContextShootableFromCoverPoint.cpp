
#include "Gameplay/CoverGenerator/EnvQuery/Tests/EnvQueryTest_IsContextShootableFromCoverPoint.h"
#include "Gameplay/CoverGenerator/CoverGenerator.h"
#include "Gameplay/CoverGenerator/EnvQuery/EnvQueryItemType_Cover.h"

#include "Runtime/AIModule/Classes/EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "Runtime/AIModule/Classes/EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"


UEnvQueryTest_IsContextShootableFromCoverPoint::UEnvQueryTest_IsContextShootableFromCoverPoint(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_Cover::StaticClass();
	SetWorkOnFloatValues(false);

	Context = UEnvQueryContext_Querier::StaticClass();
}

void UEnvQueryTest_IsContextShootableFromCoverPoint::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);

	const bool bWantsHit = BoolValue.GetValue();

	TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(Context, ContextLocations))
	{
		return;
	}

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		UCoverPoint* CoverPoint = UEnvQueryItemType_Cover::GetValue(QueryInstance.RawData.GetData() + QueryInstance.Items[It.GetIndex()].DataOffset);
		ACoverGenerator* CoverGenerator = ACoverGenerator::GetCoverGenerator(nullptr);

		if (CoverPoint)
		{
			const FVector LocalRight = FVector::CrossProduct(FVector::UpVector, CoverPoint->DirectionToWall());
			const FVector SourceLocation = CoverPoint->Location + LocalRight * CoverGenerator->GetOffsetWhenLeaningSides() +  CoverPoint->DirectionToWall() * CoverGenerator->GetOffsetFrontAim();

			for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
			{
				TArray<FHitResult> OutHits;
				bool bIsHit = QueryInstance.World->LineTraceMultiByObjectType(OutHits, SourceLocation, ContextLocations[ContextIndex], ObjectQueryParams, QueryParams);

				It.SetScore(TestPurpose, FilterType, bIsHit, !bWantsHit);
			}
		}
	}
}