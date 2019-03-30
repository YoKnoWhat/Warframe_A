
#include "Character/AISense_Sight_NoAutoRegister.h"


UAISense_Sight_NoAutoRegister::UAISense_Sight_NoAutoRegister(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	bAutoRegisterAllPawnsAsSources = false;
	bWantsNewPawnNotification = false;
}