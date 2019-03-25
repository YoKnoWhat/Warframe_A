
#include "Character/Warframe/WarframeController.h"
#include "WarframeCharacter.h"


AWarframeController::AWarframeController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{}

void AWarframeController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	Cast<AWarframeCharacter>(InPawn)->SetGenericTeamId(FGenericTeamId(CastToUnderlyingType(EWarframeTeamID::Player)));
}
