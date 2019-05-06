// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/GameMode/WarframeGameMode.h"
#include "Character/Warframe/Warframe.h"
#include "Gameplay/SpawnPoint.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Gameplay/WarframeGameInstance.h"
#include "UI/PlayerHUD.h"
#include "Utility/HelperFunction.h"

#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"


AWarframeGameMode::AWarframeGameMode(const FObjectInitializer& ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AWarframeGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector PlayerLocation = Player->GetActorLocation();

	for (ASpawnPoint* SpawnPoint : SpawnPointArray)
	{
		if (SpawnPoint->IsEmpty())
		{
			if ((PlayerLocation - SpawnPoint->GetActorLocation()).Size2D() > 500.0f)
			{
				SpawnPoint->SpawnIfEmpty();
			}
		}
	}
}

void AWarframeGameMode::RegisterSpawnPoint(class ASpawnPoint* SpawnPoint)
{
	SpawnPointArray.Add(SpawnPoint);
}

void AWarframeGameMode::OnCharacterDamaged(const FDamageInfo& DamageInfo, AWarframeCharacter* DamagedCharacter, float ActualDamage, bool IsDamageOnShield)
{
	if (DamageInfo.Causer == Player)
	{
		HUD->OnApplyDamageToEnemy(DamageInfo, ActualDamage, IsDamageOnShield);
	}

	if (DamagedCharacter == Player)
	{
		HUD->OnPlayerDamaged(IsDamageOnShield);
	}
}

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

void AWarframeGameMode::OnCharacterDied(AActor *Causer, AWarframeCharacter* Character)
{
	this->OnCharacterDiedBP(Causer, Character);
}

void AWarframeGameMode::BeginPlay()
{
	Player = Cast<AWarframe>(UGameplayStatics::GetPlayerCharacter(this, 0));

	Super::BeginPlay();

	/** Stimulate in editor. */
	if (Player != nullptr)
	{
		HUD = CreateWidget<UPlayerHUD>(this->GetGameInstance(), FWarframeConfigSingleton::Instance().FindResourceClass("UI_PlayerHUD"));
		HUD->Init(Player);

		HUD->AddToViewport();
	}

	// this->ReadInSpawnPointArray();
	// 
	// for (auto &SpawnPoint : this->SpawnPointArray)
	// {
	// 	SpawnPoint->SpawnAnyway();
	// }
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

/*void AWarframeGameMode::ReadInSpawnPointArray()
{
	FString FilePath = FPaths::ProjectContentDir() + TEXT("DataTable/SpawnPointTable.dt");

	const char* Begin;
	const char* End;
	if (Warframe::GetFileContent(FilePath, Begin, End))
	{
		// Cached zero-offset buffer begin.
		const char *BufferBegin = Begin;

		// Temp variables
		FSpawnInfo CharacterSpawnInfo;
		int32 tempInt32;

		struct FSpawnInfoExt
		{
			FVector Location;
			TArray<FSpawnInfo> CharacterSpawnInfoArray;
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
}*/
