// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/WarframeGameMode.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/SpawnPoint.h"
#include "Gameplay/WarframeGameInstance.h"
#include "Utility/HelperFunction.h"

#include "Runtime/Core/Public/Misc/Paths.h"


void AWarframeGameMode::OnCharacterDamaged_Implementation(AActor* DamageCauser, AWarframeCharacter* DamagedCharacter, FVector HitLocation, EDamageType StatusEffect, float Damage, bool IsDamageOnShield, int32 CriticalTier)
{}

void AWarframeGameMode::OnCharacterSpawned(AWarframeCharacter* Character)
{
	Character->SetOrbDropChances(0.2f, 0.2f);
	{
		TArray<EPickableObjectID> IDs;
		TArray<float> Chances;

		IDs.Add(EPickableObjectID::RifleAmmo);
		Chances.Add(0.2f);
		IDs.Add(EPickableObjectID::ShotgunAmmo);
		Chances.Add(0.2f);
		IDs.Add(EPickableObjectID::SniperAmmo);
		Chances.Add(0.2f);
		IDs.Add(EPickableObjectID::BowAmmo);
		Chances.Add(0.2f);
		IDs.Add(EPickableObjectID::PistolAmmo);
		Chances.Add(0.2f);
		Character->SetCommonDropItems(IDs, Chances);
	}
}

void AWarframeGameMode::BeginPlay()
{
	Super::BeginPlay();

	this->ReadInSpawnPointArray();

	for (auto &SpawnPoint : this->SpawnPointArray)
	{
		SpawnPoint->SpawnAnyway();
	}
}

void AWarframeGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	Super::FinishRestartPlayer(NewPlayer, StartRotation);

	AWarframeCharacter* Character = Cast<AWarframeCharacter>(NewPlayer->GetPawn());
	if (Character != nullptr)
	{
		Character->SetGenericTeamId(FGenericTeamId(CastToUnderlyingType(EWarframeTeamID::Player)));
	}
}

void AWarframeGameMode::ReadInSpawnPointArray()
{
	FString FilePath = FPaths::ProjectContentDir() + TEXT("DataTable/SpawnPointTable.dt");

	const char* Begin;
	const char* End;
	if (Warframe::GetFileContent(FilePath, Begin, End))
	{
		// Cached zero-offset buffer begin.
		const char *BufferBegin = Begin;

		// Temp variables
		FCharacterSpawnInfo CharacterSpawnInfo;
		int32 tempInt32;

		struct FSpawnInfoExt
		{
			FVector Location;
			TArray<FCharacterSpawnInfo> CharacterSpawnInfoArray;
		};
		TArray<FSpawnInfoExt> SpawnInfoArray;

		while (Begin != End)
		{
			FSpawnInfoExt &SpawnInfo = SpawnInfoArray[SpawnInfoArray.Add(FSpawnInfoExt())];

			Warframe::ReadIn(tempInt32, Begin);
			SpawnInfo.Location.X = static_cast<float>(tempInt32);
			Warframe::ReadIn(tempInt32, Begin);
			SpawnInfo.Location.Y = static_cast<float>(tempInt32);
			Warframe::ReadIn(tempInt32, Begin);
			SpawnInfo.Location.Z = static_cast<float>(tempInt32);

			for (uint32 i = 0; i < 4; ++i)
			{
				Warframe::ReadIn(CharacterSpawnInfo.CharacterID, Begin);
				Warframe::ReadIn(CharacterSpawnInfo.Level, Begin);
				Warframe::ReadIn(CharacterSpawnInfo.Number, Begin);

				if (CharacterSpawnInfo.CharacterID != ECharacterID::None)
				{
					SpawnInfo.CharacterSpawnInfoArray.Add(CharacterSpawnInfo);
				}
			}
		}

		for (int32 i = 0; i < SpawnInfoArray.Num(); ++i)
		{
			USpawnPoint* NewSpawnPoint = NewObject<USpawnPoint>(this);

			NewSpawnPoint->Init(SpawnInfoArray[i].Location, SpawnInfoArray[i].CharacterSpawnInfoArray);

			this->SpawnPointArray.Add(NewSpawnPoint);
		}
	}
}
