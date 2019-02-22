// Fill out your copyright notice in the Description page of Project Settings.

#include "WarframeGameMode.h"
#include "HelperFunction.h"
#include "SpawnPoint.h"
#include "WarframeGameInstance.h"
#include "Runtime/Core/Public/Misc/Paths.h"


void AWarframeGameMode::OnCharacterDamaged_Implementation(AActor* DamageCauser, AWarframeCharacter* DamagedCharacter, FVector HitLocation, EDamageType StatusEffect, float Damage, bool IsDamageOnShield, int32 CriticalTier)
{}

void AWarframeGameMode::BeginPlay()
{
	Super::BeginPlay();

	this->ReadInSpawnPointArray();

	for (auto &SpawnPoint : this->SpawnPointArray)
	{
		SpawnPoint->SpawnAnyway();
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

		FVector Location;
		TArray<FCharacterSpawnInfo> SpawnInfoArray;

		while (Begin != End)
		{
			Warframe::ReadIn(tempInt32, Begin);
			Location.X = static_cast<float>(tempInt32);
			Warframe::ReadIn(tempInt32, Begin);
			Location.Y = static_cast<float>(tempInt32);
			Warframe::ReadIn(tempInt32, Begin);
			Location.Z = static_cast<float>(tempInt32);

			for (uint32 i = 0; i < 4; ++i)
			{
				Warframe::ReadIn(CharacterSpawnInfo.CharacterID, Begin);
				Warframe::ReadIn(CharacterSpawnInfo.Level, Begin);
				Warframe::ReadIn(CharacterSpawnInfo.Number, Begin);

				if (CharacterSpawnInfo.CharacterID != ECharacterID::None)
				{
					SpawnInfoArray.Add(CharacterSpawnInfo);
				}
			}
		}

		if (SpawnInfoArray.Num() != 0)
		{
			USpawnPoint* NewSpawnPoint = NewObject<USpawnPoint>(this);

			NewSpawnPoint->Init(Location, SpawnInfoArray);

			this->SpawnPointArray.Add(NewSpawnPoint);
		}
	}
}
