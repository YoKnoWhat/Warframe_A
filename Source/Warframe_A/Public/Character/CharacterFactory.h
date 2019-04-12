
#pragma once

#include "WarframeCommon.h"


class AActor;

class FCharacterFactory
{
public:
	static FCharacterFactory& Instance();

	template<class T>
	T* SpawnCharacter(AActor* Owner, ECharacterID CharacterID, const FTransform& Transform)
	{
		return Cast<T>(this->SpawnCharacterImpl(Owner, CharacterID, Transform));
	}

	void SetOverride(ECharacterID CharacterID, UClass* OverrideClass);

protected:
	AWarframeCharacter* SpawnCharacterImpl(AActor* Owner, ECharacterID CharacterID, const FTransform& Transform);

protected:
	TMap<ECharacterID, UClass*> OverrideClasses;
};