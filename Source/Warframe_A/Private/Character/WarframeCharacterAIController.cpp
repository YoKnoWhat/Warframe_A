
#include "Character/WarframeCharacterAIController.h"
#include "Character/AITargetSelectionComponent.h"
#include "Character/WarframeCharacter.h"
#include "Character/AISense_Sight_NoAutoRegister.h"

#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Hearing.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Team.h"


AWarframeCharacterAIController::AWarframeCharacterAIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	/** Target selection component creation. */
	TargetSelection = ObjectInitializer.CreateDefaultSubobject<UAITargetSelectionComponent>(this, "TargetSelection");

	/** AI perception component creation. */
	UAIPerceptionComponent* AIPerception = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, "AIPerception");
	this->SetPerceptionComponent(*AIPerception);

	AIPerception->SetDominantSense(UAISense_Sight_NoAutoRegister::StaticClass());

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
		SenseConfig_Sight->Implementation = UAISense_Sight_NoAutoRegister::StaticClass();
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

	// todo: for now.
	this->SetGenericTeamId(FGenericTeamId(CastToUnderlyingType(EWarframeTeamID::Enemy1)));
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
