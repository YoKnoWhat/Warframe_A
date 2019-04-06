
#include "Environment/CoverGenerator/EnvQuery/EnvQueryInstanceWarframeWrapper.h"
#include "Environment/CoverGenerator/EnvQuery/EnvQueryItemType_Cover.h"
#include "Environment/CoverGenerator/CoverPoint.h"


TArray<UCoverPoint*> UEnvQueryInstanceWarframeWrapper::GetResultsAsCoverPoints()
{
	TArray<UCoverPoint*> Results;

	if (QueryResult.IsValid() && ItemType->IsChildOf(UEnvQueryItemType_Cover::StaticClass()))
	{
		UEnvQueryItemType_Cover* DefTypeOb = ItemType->GetDefaultObject<UEnvQueryItemType_Cover>();

		if (RunMode != EEnvQueryRunMode::AllMatching)
		{
			if (QueryResult->Items.IsValidIndex(0))
			{
				Results.Add(DefTypeOb->GetValue(QueryResult->RawData.GetData() + QueryResult->Items[0].DataOffset));
			}
		}
		else
		{
			Results.Reserve(QueryResult->Items.Num());

			for (auto& Item : QueryResult->Items)
			{
				Results.Add(DefTypeOb->GetValue(QueryResult->RawData.GetData() + Item.DataOffset));
			}
		}
	}

	return Results;
}