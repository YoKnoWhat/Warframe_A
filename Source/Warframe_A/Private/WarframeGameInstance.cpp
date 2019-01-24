// Fill out your copyright notice in the Description page of Project Settings.

#include "WarframeGameInstance.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"


UWarframeGameInstance::UWarframeGameInstance(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{}

void UWarframeGameInstance::Init()
{
	FString AbsolutePath = FPaths::ProjectContentDir() + TEXT("DataTable/EnemyTable.dt");

	IFileHandle *EnemyTableFile = FPlatformFileManager::Get().GetPlatformFile().OpenRead(*AbsolutePath);

	ECharacterID CharacterID;

	while (EnemyTableFile->Read(reinterpret_cast<uint8*>(&CharacterID), sizeof(ECharacterID)))
	{
		FCharacterProperties& newCharacterProp = CharacterPropTable.Add(CharacterID);

		EnemyTableFile->Read(reinterpret_cast<uint8*>(&newCharacterProp), sizeof(FCharacterProperties));
	}

	delete EnemyTableFile;
}

const FCharacterProperties *UWarframeGameInstance::GetCharacterProp(ECharacterID CharacterID)const
{
	return CharacterPropTable.Find(CharacterID);
}

const FName UWarframeGameInstance::GetCharacterName(ECharacterID CharacterID)const
{
	static const FName ECharacterIDNames[] = {
		// Light Grineer units
		"Butcher",
		"Flameblade",
		"Guardsman",
		"Powerfist",
		"Scorpion",
		"ShieldLancer",

		// Medium Grineer units
		"Ballista",
		"EliteLancer",
		"Eviscerator",
		"Hellion",
		"Lancer",
		"Scorch",
		"Seeker",
		"Trooper",

		// Heavy Grineer units
		"Bailiff",
		"Bombard",
		"Commander",
		"DrahkMaster",
		"HeavyGunner",
		"HyekkaMaster",
		// Manic,
		"Napalm",
		"Nox",

		// ...

		// Corpus crewmen
		"Crewman",
		"DetronCrewman",
		"SniperCrewman",
		"EliteCrewman",
		"Tech",
		"ProdCrewman",
		"NullifierCrewman",
		"Comba",
		"Scrambus",

		// Corpus walkers
		"Moa",
		"ShockwaveMoa",
		"RailgunMoa",
		"FusionMoa",
		"AntiMoa",
		"IsolatorBursa",
		"DroverBursa",
		"DenialBursa",

		// Corpus ospreys
		"LeechOsprey",
		"MineOsprey",
		"OxiumOsprey",
		"ShieldOsprey",
		"SappingOsprey",
		"AttackDrone",
		"ScavengerDrone",

		// ...
	};

	return ECharacterIDNames[static_cast<uint32>(CharacterID)];
}
