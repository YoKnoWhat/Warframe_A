// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/SpawnPoint.h"
#include "Character/CharacterFactory.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Gameplay/WarframeGameInstance.h"
#include "Gameplay/WarframeGameMode.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponFactory.h"

#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
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
			FTransform CharacterTransform(FRotator::ZeroRotator, Location);

			AWarframeCharacter *NewCharacter = FCharacterFactory::Instance().SpawnCharacter<AWarframeCharacter>(GameMode, SpawnInfo.CharacterID, CharacterTransform);
			if (NewCharacter != nullptr)
			{
				this->SpawnedCharacters.Add(NewCharacter);

				NewCharacter->SetLevel(SpawnInfo.Level);

				/** Set character appearance. */
				const FCharacterAppearance* CharacterAppearance = GameInstance->GetCharacterAppearance(SpawnInfo.CharacterID);

				USkeletalMeshComponent *SkeletalMeshComponent = Cast<USkeletalMeshComponent>(NewCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
				SkeletalMeshComponent->SetSkeletalMesh(FWarframeConfigSingleton::Instance().FindResource<USkeletalMesh>(*CharacterAppearance->Mesh.ToString()));
				SkeletalMeshComponent->SetRelativeLocation(CharacterAppearance->RelativeLocation);
				SkeletalMeshComponent->SetRelativeRotation(CharacterAppearance->RelativeRotation);
				SkeletalMeshComponent->SetAnimInstanceClass(FWarframeConfigSingleton::Instance().FindResourceClass(*CharacterAppearance->AnimClass.ToString()));

				UCapsuleComponent* CapsuleComponent = NewCharacter->GetCapsuleComponent();
				CapsuleComponent->SetCapsuleHalfHeight(CharacterAppearance->HalfHeight);
				CapsuleComponent->SetCapsuleRadius(CharacterAppearance->Radius);

				/** Set character weapons. */
				const FEnemyInfo* EnemyInfo = GameInstance->GetEnemyInfo(SpawnInfo.CharacterID);
				EnemyInfo->Faction;

				AWeaponBase* Weapon1 = FWeaponFactory::Instance().SpawnWeapon<AWeaponBase>(NewCharacter, EnemyInfo->Weapon1, FTransform());
				if (Weapon1 != nullptr)
				{
					NewCharacter->SetWeapon(EWeaponSlotType::Primary, Weapon1);
					Weapon1->AttachToComponent(NewCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Socket_RightHand");
				}

				AWeaponBase* Weapon2 = FWeaponFactory::Instance().SpawnWeapon<AWeaponBase>(NewCharacter, EnemyInfo->Weapon2, FTransform());
				if (Weapon2 != nullptr)
				{
					NewCharacter->SetWeapon(EWeaponSlotType::Secondary, Weapon2);
					Weapon2->AttachToComponent(NewCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Socket_RightHand");
				}

				NewCharacter->SwitchToRangedWeapon();

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
