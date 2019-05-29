
#include "Character/Corpus/CorpusAITargetSelectionComponent.h"
#include "Character/Corpus/CorpusAIController.h"
#include "Character/WarframeCharacter.h"

#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"


void UCorpusAITargetSelectionComponent::UpdateSelectedTarget()
{
	ACorpusAIController* Controller = Cast<ACorpusAIController>(GetOwner());
	AWarframeCharacter* Target = Cast<AWarframeCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject("Target"));

	if (Target != nullptr)
	{
		FVector FakeHitLocation = Target->GetActorLocation();
		FVector FakeHitNormal = Target->GetActorLocation() - Controller->GetPawn()->GetActorLocation();

		SelectedTarget = FHitResult(Target, Target->GetCapsuleComponent(), FakeHitLocation, FakeHitNormal.GetUnsafeNormal());
	}

	/*ACorpusAIController* Controller = Cast<ACorpusAIController>(GetOwner());

	AWarframeCharacter* Target = Cast<AWarframeCharacter>(Controller->GetBlackboardComponent()->GetValueAsObject("Target"));
	if (Target != nullptr)
	{
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;
		QueryParams.AddIgnoredActor(Controller->GetPawn());

		if (GetWorld()->LineTraceSingleByObjectType(
			SelectedTarget,
			Controller->GetPawn()->GetActorLocation(),
			Target->GetActorLocation(),
			ObjectQueryParams,
			QueryParams) == true)
		{
			if (SelectedTarget.Actor.Get() != Target)
			{
				SelectedTarget = FHitResult(FVector(), FVector());
			}
		}
	}*/
}
