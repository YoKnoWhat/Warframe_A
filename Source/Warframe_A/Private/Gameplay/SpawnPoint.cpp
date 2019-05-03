// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/SpawnPoint.h"
#include "Character/CharacterFactory.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/GameMode/WarframeGameMode.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Gameplay/WarframeGameInstance.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/WeaponFactory.h"

#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"


DECLARE_DELEGATE_OneParam(FCharacterOnDestroyedDelegate, AActor*)

ASpawnPoint::ASpawnPoint(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	/** In case that default game mode is none. */
	AWarframeGameMode* GameMode = Cast<AWarframeGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		GameMode->RegisterSpawnPoint(this);
	}
}

void ASpawnPoint::SpawnAnyway()
{
	this->KillAll();

	this->SpawnIfEmpty();
}

void ASpawnPoint::SpawnIfEmpty()
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

		ECharacterID CharacterID = static_cast<ECharacterID>(SpawnInfo.CharacterID);

		for (int32 SpawnIndex = 0; SpawnIndex < SpawnInfo.Number; ++SpawnIndex)
		{
			FTransform CharacterTransform(FRotator::ZeroRotator, this->GetActorLocation());

			AWarframeCharacter *NewCharacter = FCharacterFactory::Instance().SpawnCharacter<AWarframeCharacter>(GameMode, CharacterID, CharacterTransform);
			if (NewCharacter != nullptr)
			{
				this->SpawnedCharacters.Add(NewCharacter);

				NewCharacter->SetLevel(SpawnInfo.Level);
				NewCharacter->SetGenericTeamId(FGenericTeamId(CastToUnderlyingType(EWarframeTeamID::Enemy1)));

				/** Set character weapons. */
				const FEnemyInfo* EnemyInfo = GameInstance->GetEnemyInfo(CharacterID);
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
				NewCharacter->OnDied.AddUObject(this, &ASpawnPoint::OnCharacterDied);
				NewCharacter->OnDied.AddUObject(GameMode, &AWarframeGameMode::OnCharacterDied);

				GameMode->OnCharacterSpawned(NewCharacter);				
			}
		}
	}
}

void ASpawnPoint::KillAll()
{
	for (AWarframeCharacter *Character : SpawnedCharacters)
	{
		Character->Destroy();
	}
}

bool ASpawnPoint::IsEmpty()const
{
	return SpawnedCharacters.Num() == 0;
}

void ASpawnPoint::OnCharacterDied(AActor* Causer, AWarframeCharacter* Character)
{
	SpawnedCharacters.Remove(Cast<AWarframeCharacter>(Character));
}
