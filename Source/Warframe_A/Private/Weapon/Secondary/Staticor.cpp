
#include "Weapon/Secondary/Staticor.h"
#include "Weapon/Secondary/StaticorRound.h"
#include "Weapon/TriggerModifier.h"

#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"


AStaticor::AStaticor(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<USkeletalMeshComponent>("Mesh"))
{}

void AStaticor::Init(EWeaponID WeaponID, uint32 InLevel)
{
	Super::Init(WeaponID, InLevel);

	// this->MaxChargedRounds = 4;
}

UClass* AStaticor::GetRoundClass_Implementation()const
{
	return AStaticorRound::StaticClass();
}

bool AStaticor::IsFullCharged()const
{
	FTriggerModifier_Charge* ChargeTrigger = dynamic_cast<FTriggerModifier_Charge*>(GetTriggerModifier());

	return ChargeTrigger->GetCurrentChargeRounds() == ChargeTrigger->GetMaxChargeRounds();
}
