
#include "Character/Warframe/Excalibur/Excalibur.h"
#include "Character/Warframe/Excalibur/ExaltedBlade.h"
#include "Character/Warframe/Excalibur/RadialBlind.h"
#include "Character/Warframe/Excalibur/RadialJavelin.h"
#include "Character/Warframe/Excalibur/SlashDash.h"


void AExcalibur::BeginPlay()
{
	Super::BeginPlay();

	Abilities[0] = new FAbilityObject_Null();
	Abilities[1] = new FAbilityObject_Null();
	Abilities[2] = new FRadialJavelin(this);
	Abilities[3] = new FAbilityObject_Null();
}

void AExcalibur::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExcalibur::Init(ECharacterID CharacterID, uint32 Level_)
{
	Super::Init(CharacterID, Level_);
}

void AExcalibur::OnLevelChanged()
{
	Super::OnLevelChanged();
}
