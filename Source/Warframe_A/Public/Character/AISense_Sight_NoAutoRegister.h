
#pragma once

#include "Runtime/AIModule/Classes/Perception/AISense_Sight.h"
#include "AISense_Sight_NoAutoRegister.generated.h"


UCLASS()
class WARFRAME_A_API UAISense_Sight_NoAutoRegister : public UAISense_Sight
{
	GENERATED_BODY()

public:
	UAISense_Sight_NoAutoRegister(const FObjectInitializer& ObjectInitializer);
};