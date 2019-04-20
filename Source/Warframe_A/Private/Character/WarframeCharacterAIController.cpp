
#include "Character/WarframeCharacterAIController.h"
#include "Character/AITargetSelectionComponent.h"
#include "Character/StateMachine/WarframeCharacterAIStateMachineComponent.h"
#include "Character/WarframeCharacter.h"

#include "Runtime/AIModule/Classes/Navigation/CrowdFollowingComponent.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Hearing.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Team.h"


AWarframeCharacterAIController::AWarframeCharacterAIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent")
							.SetDefaultSubobjectClass<UAITargetSelectionComponent>("TargetSelection"))
{
	/** Target selection component creation. */
	TargetSelection = ObjectInitializer.CreateDefaultSubobject<UAITargetSelectionComponent>(this, "TargetSelection");

	/** AI perception component creation. */
	UAIPerceptionComponent* AIPerception = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, "AIPerception");
	this->SetPerceptionComponent(*AIPerception);

	AIPerception->SetDominantSense(UAISense_Sight::StaticClass());

	UAISenseConfig_Hearing* SenseConfig_Hearing = NewObject<UAISenseConfig_Hearing>(AIPerception, "AISenseConfig_Hearing");
	{
		SenseConfig_Hearing->Implementation = UAISense_Hearing::StaticClass();
		SenseConfig_Hearing->HearingRange = 3000.0f;
		SenseConfig_Hearing->bUseLoSHearing = false;
		SenseConfig_Hearing->DetectionByAffiliation.bDetectEnemies = true;
		SenseConfig_Hearing->DetectionByAffiliation.bDetectNeutrals = false;
		SenseConfig_Hearing->DetectionByAffiliation.bDetectFriendlies = false;
	}
	AIPerception->ConfigureSense(*SenseConfig_Hearing);

	UAISenseConfig_Sight* SenseConfig_Sight = NewObject<UAISenseConfig_Sight>(AIPerception, "AISenseConfig_Sight");
	{
		SenseConfig_Sight->Implementation = UAISense_Sight::StaticClass();
		SenseConfig_Sight->SightRadius = 3000.0f;
		SenseConfig_Sight->LoseSightRadius = 3500.0f;
		SenseConfig_Sight->PeripheralVisionAngleDegrees = 45.0f;
		SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
		SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
		SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
		SenseConfig_Sight->AutoSuccessRangeFromLastSeenLocation = -1.000000f;
	}
	AIPerception->ConfigureSense(*SenseConfig_Sight);

	UAISenseConfig_Team* SenseConfig_Team = NewObject<UAISenseConfig_Team>(AIPerception, "AISenseConfig_Team");
	{
	}
	AIPerception->ConfigureSense(*SenseConfig_Team);
}

void AWarframeCharacterAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	Cast<AWarframeCharacter>(InPawn)->SetGenericTeamId(FGenericTeamId(CastToUnderlyingType(EWarframeTeamID::Enemy1)));
}

void AWarframeCharacterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TargetSelection->UpdateSelectedTarget();
}

void AWarframeCharacterAIController::BeginFire()
{
	UWarframeCharacterAIStateMachineComponent* StateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine());

	StateMachine->IsFiring = true;
}

void AWarframeCharacterAIController::StopFire()
{
	UWarframeCharacterAIStateMachineComponent* StateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine());

	StateMachine->IsFiring = false;
}

void AWarframeCharacterAIController::Reload()
{
	UWarframeCharacterAIStateMachineComponent* StateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine());

	StateMachine->TriggerEvent(CastToUnderlyingType(EWarframeCharacterActionEvent::Reload));
}

void AWarframeCharacterAIController::BeginSprint()
{
	UWarframeCharacterAIStateMachineComponent* StateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine());

	StateMachine->IsSprinting = true;
}

void AWarframeCharacterAIController::StopSprint()
{
	UWarframeCharacterAIStateMachineComponent* StateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine());

	StateMachine->IsSprinting = false;
}

void AWarframeCharacterAIController::TakeCover(UCoverPoint* InCoverPoint)
{
	UWarframeCharacterAIStateMachineComponent* StateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine());
	
	StateMachine->CoverPoint = InCoverPoint;
}

void AWarframeCharacterAIController::LeaveCover()
{
	UWarframeCharacterAIStateMachineComponent* StateMachine = Cast<UWarframeCharacterAIStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine());

	StateMachine->CoverPoint = nullptr;
}
