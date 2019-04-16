
#pragma once

#include "Character/WarframeCharacterAIController.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionTypes.h"
#include "CorpusAIController.generated.h"


UCLASS()
class WARFRAME_A_API ACorpusAIController : public AWarframeCharacterAIController
{
	GENERATED_BODY()

public:
	ACorpusAIController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay()override;

	virtual void Possess(APawn* InPawn)override;

	virtual void BeginFire()override;

	virtual void StopFire()override;

	virtual void Reload()override;

protected:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	class ACorpus* Corpus;
};