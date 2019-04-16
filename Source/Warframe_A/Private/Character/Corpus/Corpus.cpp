
#include "Character/Corpus/Corpus.h"
#include "Character/Corpus/CorpusAIController.h"
#include "Character/Corpus/StateMachine/CorpusStateMachineComponent.h"
#include "Character/Corpus/StateMachine/CorpusLowerStates.h"
#include "Character/Corpus/StateMachine/CorpusUpperStates.h"


ACorpus::ACorpus(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCorpusStateMachineComponent>("StateMachine"))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->AIControllerClass = ACorpusAIController::StaticClass();
	
	StateMachineComponent->ClearAllLayers();
	StateMachineComponent->AddStateObject(CastToUnderlyingType(ECorpusStateLayer::Lower), new FCorpusLowerState_Crouching);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(ECorpusStateLayer::Lower), new FCorpusLowerState_Falling);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(ECorpusStateLayer::Lower), new FCorpusLowerState_Idle);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(ECorpusStateLayer::Lower), new FCorpusLowerState_Jumping);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(ECorpusStateLayer::Lower), new FCorpusLowerState_Sprinting);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(ECorpusStateLayer::Upper), new FCorpusUpperState_Firing);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(ECorpusStateLayer::Upper), new FCorpusUpperState_Idle);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(ECorpusStateLayer::Upper), new FCorpusUpperState_Ironsight);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(ECorpusStateLayer::Upper), new FCorpusUpperState_Reloading);
}

void ACorpus::BeginPlay()
{
	Super::BeginPlay();

	TMap<int32, int32> InitStateIDs = {
		{ CastToUnderlyingType(ECorpusStateLayer::Lower), CastToUnderlyingType(ECorpusLowerState::Idle) },
		{ CastToUnderlyingType(ECorpusStateLayer::Upper), CastToUnderlyingType(ECorpusUpperState::Idle) },
	};

	this->StateMachineComponent->Init(InitStateIDs);
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
