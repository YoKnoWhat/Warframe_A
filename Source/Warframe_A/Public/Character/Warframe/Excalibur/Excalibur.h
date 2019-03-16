
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

	virtual void Init(ECharacterID CharacterID, uint32 Level)override;

	virtual void OnLevelChanged()override;

protected:

};
