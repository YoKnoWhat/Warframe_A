
#include "Character/CharacterFactory.h"
#include "Character/Corpus/Corpus.h"
#include "Character/Warframe/Warframe.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Gameplay/WarframeGameInstance.h"

#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
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

	UClass** Result = ClassOverrides.Find(CharacterID);
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

		/** Set character appearance. */
		const FCharacterAppearance* CharacterAppearance = Cast<UWarframeGameInstance>(Owner->GetGameInstance())->GetCharacterAppearance(CharacterID);

		USkeletalMeshComponent *SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Character->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		SkeletalMeshComponent->SetSkeletalMesh(FWarframeConfigSingleton::Instance().FindResource<USkeletalMesh>(*CharacterAppearance->Mesh.ToString()));
		SkeletalMeshComponent->SetRelativeLocation(CharacterAppearance->RelativeLocation);
		SkeletalMeshComponent->SetRelativeRotation(CharacterAppearance->RelativeRotation);
		SkeletalMeshComponent->SetAnimInstanceClass(FWarframeConfigSingleton::Instance().FindResourceClass(*CharacterAppearance->AnimClass.ToString()));

		UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
		CapsuleComponent->SetCapsuleHalfHeight(CharacterAppearance->HalfHeight);
		CapsuleComponent->SetCapsuleRadius(CharacterAppearance->Radius);
	}

	Character->Init(CharacterID);
	return Character;
}

void FCharacterFactory::SetOverride(ECharacterID CharacterID, UClass* OverrideClass)
{
	ClassOverrides.Add(CharacterID, OverrideClass);
}

void FCharacterFactory::ClearOverrides()
{
	ClassOverrides.Empty();
}
