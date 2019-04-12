
#pragma once

#include "Weapon/RoundBase.h"
#include "RoundInstant.generated.h"


UCLASS()
class WARFRAME_A_API ARoundInstant : public ARoundBase
{
	GENERATED_BODY()

public:
	ARoundInstant(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime)override;
};
