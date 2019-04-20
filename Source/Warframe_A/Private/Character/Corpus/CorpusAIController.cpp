
#include "Character/Corpus/CorpusAIController.h"
#include "Character/Corpus/Corpus.h"
#include "Character/Corpus/CorpusAITargetSelectionComponent.h"
#include "Character/Corpus/StateMachine/CorpusStateMachineComponent.h"
#include "Gameplay/WarframeConfigSingleton.h"

#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Hearing.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Sight.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


ACorpusAIController::ACorpusAIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCorpusAITargetSelectionComponent>("TargetSelection"))
{}

void ACorpusAIController::BeginPlay()
{
	Super::BeginPlay();

	UBlackboardComponent* BBPointer;
	UseBlackboard(FWarframeConfigSingleton::Instance().FindResource<UBlackboardData>("BB_CorpusBlackboard"), BBPointer);

	GetAIPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACorpusAIController::OnTargetPerceptionUpdated);
}

void ACorpusAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	this->Corpus = Cast<ACorpus>(InPawn);

	this->Blackboard->SetValueAsVector("HomeLocation", InPawn->GetActorLocation());
	this->Blackboard->SetValueAsVector("NoiseLocation", FVector::ZeroVector);

	RunBehaviorTree(FWarframeConfigSingleton::Instance().FindResource<UBehaviorTree>("BT_CorpusBehaviorTree"));
}

void ACorpusAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			this->Blackboard->SetValueAsVector("NoiseLocation", Stimulus.StimulusLocation);
		}
		else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			this->Blackboard->SetValueAsObject("Target", Actor);
		}
	}
}
