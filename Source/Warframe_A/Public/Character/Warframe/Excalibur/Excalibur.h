
#pragma once

#include "Character/Warframe/Warframe.h"
#include "Excalibur.generated.h"


UCLASS(BlueprintType)
class AExcalibur : public AWarframe
{
	GENERATED_BODY()

public:
	AExcalibur(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	virtual void SetLevel(uint32 InLevel)override;

protected:
	virtual void Init(ECharacterID InCharacterID)override;
};
