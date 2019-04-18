
#include "Character/Corpus/Corpus.h"
#include "Character/Corpus/CorpusAIController.h"
#include "Character/Corpus/StateMachine/CorpusStateMachineComponent.h"
#include "Character/Corpus/StateMachine/CorpusLowerStates.h"
#include "Character/Corpus/StateMachine/CorpusUpperStates.h"


ACorpus::ACorpus(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCorpusStateMachineComponent>(AWarframeCharacter::StateMachineName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->AIControllerClass = ACorpusAIController::StaticClass();
}

void ACorpus::BeginPlay()
{
	Super::BeginPlay();
}

void ACorpus::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACorpus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACorpus::Init(ECharacterID InCharacterID)
{
	Super::Init(InCharacterID);
}
