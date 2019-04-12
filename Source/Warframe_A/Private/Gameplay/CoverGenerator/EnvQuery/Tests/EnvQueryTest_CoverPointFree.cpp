
#include "Gameplay/CoverGenerator/EnvQuery/Tests/EnvQueryTest_CoverPointFree.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/CoverGenerator/CoverPoint.h"
#include "Gameplay/CoverGenerator/EnvQuery/EnvQueryItemType_Cover.h"
#include "Gameplay/CoverGenerator/CoverGenerator.h"


UEnvQueryTest_CoverPointFree::UEnvQueryTest_CoverPointFree(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_Cover::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_CoverPointFree::RunTest(FEnvQueryInstance& QueryInstance) const
{
	BoolValue.BindData(QueryInstance.Owner.Get(), QueryInstance.QueryID);
	bool DesiredBool = BoolValue.GetValue();

	// Don't support context Item here, it doesn't make any sense.
	TArray<AActor*> ContextActors;
	if (!QueryInstance.PrepareContext(ConsiderFree, ContextActors))
	{
		return;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		// Get current CoverPoint
		UCoverPoint* CurrentCoverPoint = UEnvQueryItemType_Cover::GetValue(QueryInstance.RawData.GetData() + QueryInstance.Items[It.GetIndex()].DataOffset);

		AActor* Occupant = CurrentCoverPoint->GetOccupant();
		bool IsFree = Occupant == nullptr;

		if (IsFree == false)
		{
			for (auto Actor : ContextActors)
			{
				if (Occupant == Actor)
				{
					IsFree = true;
					break;
				}
			}
		}

		It.SetScore(TestPurpose, FilterType, IsFree, DesiredBool);
	}
}
