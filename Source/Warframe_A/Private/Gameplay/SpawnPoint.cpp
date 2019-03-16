// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/SpawnPoint.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/WarframeGameMode.h"
#include "Gameplay/WarframeGameInstance.h"

#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"


DECLARE_DELEGATE_OneParam(FCharacterOnDestroyedDelegate, AActor*)

void USpawnPoint::Init(const FVector &Location_, const TArray<FCharacterSpawnInfo> &SpawnInfoArray_)
{
	this->Location = Location_;
	this->SpawnInfoArray = SpawnInfoArray_;
}

void USpawnPoint::SpawnAnyway()
{
	this->KillAll();

	this->SpawnIfAllKilled();
}

void USpawnPoint::SpawnIfAllKilled()
{
	if (this->SpawnedCharacters.Num() != 0)
	{
		return;
	}

	UWarframeGameInstance* GameInstance = Cast<UWarframeGameInstance>(this->GetWorld()->GetGameInstance());
	AWarframeGameMode* GameMode = Cast<AWarframeGameMode>(this->GetWorld()->GetAuthGameMode());

	for (auto &SpawnInfo : this->SpawnInfoArray)
	{
		// todo: Use template to spawn character

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GameMode;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		for (uint32 SpawnIndex = 0; SpawnIndex < SpawnInfo.Number; ++SpawnIndex)
		{
			AWarframeCharacter *NewCharacter = this->GetWorld()->SpawnActor<AWarframeCharacter>(this->Location, FRotator(), SpawnParams);
			if (NewCharacter != nullptr)
			{
				this->SpawnedCharacters.Add(NewCharacter);

				/** Set character appearance. */
				const FCharacterAppearance* CharacterAppearance = GameInstance->GetCharacterAppearance(static_cast<ECharacterID>(SpawnInfo.CharacterID));

				USkeletalMeshComponent *SkeletalMeshComponent = Cast<USkeletalMeshComponent>(NewCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
				SkeletalMeshComponent->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, *CharacterAppearance->Mesh.ToString()));
				SkeletalMeshComponent->SetRelativeLocation(CharacterAppearance->RelativeLocation);
				SkeletalMeshComponent->SetRelativeRotation(CharacterAppearance->RelativeRotation);
				SkeletalMeshComponent->SetAnimInstanceClass(LoadClass<UAnimInstance>(nullptr, *CharacterAppearance->AnimClass.ToString()));

				UCapsuleComponent* CapsuleComponent = NewCharacter->GetCapsuleComponent();
				CapsuleComponent->SetCapsuleHalfHeight(CharacterAppearance->HalfHeight);
				CapsuleComponent->SetCapsuleRadius(CharacterAppearance->Radius);

				NewCharacter->Init(static_cast<ECharacterID>(SpawnInfo.CharacterID), SpawnInfo.Level);

				/** Bind event to ACharacter::OnDestroyed(). */
				NewCharacter->OnDied.AddUObject(this, &USpawnPoint::OnCharacterDied);
				NewCharacter->OnDied.AddUObject(GameMode, &AWarframeGameMode::OnCharacterDied);

				GameMode->OnCharacterSpawned(NewCharacter);				
			}
		}
	}
}

void USpawnPoint::KillAll()
{
	for (AWarframeCharacter *Character : SpawnedCharacters)
	{
		Character->Destroy();
	}
}

void USpawnPoint::OnCharacterDied(AActor* Causer, AWarframeCharacter* Character)
{
	SpawnedCharacters.Remove(Cast<AWarframeCharacter>(Character));
}
