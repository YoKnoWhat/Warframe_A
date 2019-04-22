
#pragma once

#include "Runtime/Engine/Classes/Engine/LevelScriptActor.h"
#include "WarframeLevelScriptActor.generated.h"


UCLASS()
class WARFRAME_A_API AWarframeLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	class APostProcessVolume* GetUnboundPostProcessVolume();

protected:
	UPROPERTY(BlueprintReadWrite)
	class APostProcessVolume* UnboundPPVolume;
};