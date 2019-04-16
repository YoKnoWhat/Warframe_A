
#include "Character/CharacterFactory.h"
#include "Character/Corpus/Corpus.h"
#include "Character/Warframe/Warframe.h"

#include "Runtime/Engine/Classes/Engine/World.h"


FCharacterFactory& FCharacterFactory::Instance()
{
	static FCharacterFactory Inst;
	return Inst;
}

AWarframeCharacter* FCharacterFactory::SpawnCharacterImpl(AActor* Owner, ECharacterID CharacterID, const FTransform& Transform)
{
	AWarframeCharacter* Character;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UClass** Result = OverrideClasses.Find(CharacterID);
	if (Result != nullptr)
	{
		Character = Owner->GetWorld()->SpawnActor<AWarframeCharacter>(*Result, Transform, SpawnParams);
	}
	else
	{
		if (ECharacterID::Grineer_Begin <= CharacterID && CharacterID <= ECharacterID::Grineer_End)
		{
			// todo: you know
			Character = Owner->GetWorld()->SpawnActor<AWarframeCharacter>(AWarframeCharacter::StaticClass(), Transform, SpawnParams);
		}
		else if (ECharacterID::Corpus_Begin <= CharacterID && CharacterID <= ECharacterID::Corpus_End)
		{
			Character = Owner->GetWorld()->SpawnActor<AWarframeCharacter>(ACorpus::StaticClass(), Transform, SpawnParams);
		}
		else if (ECharacterID::Warframe_Begin <= CharacterID && CharacterID <= ECharacterID::Warframe_End)
		{
			Character = Owner->GetWorld()->SpawnActor<AWarframeCharacter>(AWarframe::StaticClass(), Transform, SpawnParams);
		}
		else
		{
			Character = Owner->GetWorld()->SpawnActor<AWarframeCharacter>(AWarframeCharacter::StaticClass(), Transform, SpawnParams);
		}
	}

	Character->Init(CharacterID);
	return Character;
}

void FCharacterFactory::SetOverride(ECharacterID CharacterID, UClass* OverrideClass)
{
	OverrideClasses.Add(CharacterID, OverrideClass);
}