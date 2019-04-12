
#include "Weapon/Primary/BratonPrime.h"
#include "Weapon/RoundInstant.h"


ABratonPrime::ABratonPrime(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{}

UClass* ABratonPrime::GetRoundClass_Implementation()const
{
	return ARoundInstant::StaticClass();
}
