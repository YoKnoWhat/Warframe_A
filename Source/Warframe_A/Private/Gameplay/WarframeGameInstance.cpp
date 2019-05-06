// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/WarframeGameInstance.h"
#include "Character/CharacterFactory.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "Weapon/WeaponFactory.h"
#include "Utility/HelperFunction.h"

#include "Runtime/AIModule/Classes/GenericTeamAgentInterface.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/HAL/UnrealMemory.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "Runtime/Json/Public/Serialization/JsonReader.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"


ETeamAttitude::Type WarframeAttitudeSolver(FGenericTeamId A, FGenericTeamId B)
{
	if (A == CastToUnderlyingType(EWarframeTeamID::Neutral) || A == FGenericTeamId::NoTeam
		|| B == CastToUnderlyingType(EWarframeTeamID::Neutral) || B == FGenericTeamId::NoTeam)
	{
		return ETeamAttitude::Neutral;
	}
	else
	{
		return A != B ? ETeamAttitude::Hostile : ETeamAttitude::Friendly;
	}
}



UWarframeGameInstance::UWarframeGameInstance(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{}

void UWarframeGameInstance::Init()
{
	FWarframeConfigSingleton::Instance().LoadConfig();

	this->ReadInDataTables();

	this->InitFactoryClassOverrides();

	FGenericTeamId::SetAttitudeSolver(WarframeAttitudeSolver);
}

void UWarframeGameInstance::Shutdown()
{}

const FCharacterInfo *UWarframeGameInstance::GetCharacterInfo(ECharacterID CharacterID)const
{
	return &CharacterInfoArray[static_cast<int32>(CharacterID)];
}

const FCharacterAppearance *UWarframeGameInstance::GetCharacterAppearance(ECharacterID CharacterID)const
{
	return &CharacterAppearanceArray[static_cast<int32>(CharacterID)];
}

const FEnemyInfo* UWarframeGameInstance::GetEnemyInfo(ECharacterID CharacterID)const
{
	return &EnemyInfoArray[static_cast<int32>(CharacterID)];
}

const FWarframeInfo* UWarframeGameInstance::GetWarframeInfo(ECharacterID CharacterID)const
{
	// Mapping character id to warframe id.
	int32 WarframeID = CastToUnderlyingType(CharacterID) - CastToUnderlyingType(ECharacterID::Enemy_End) - 1;
	
	return &WarframeInfoArray[WarframeID];
}

const FWeaponInfo* UWarframeGameInstance::GetWeaponInfo(EWeaponID WeaponID)const
{
	return &WeaponInfoArray[static_cast<int32>(WeaponID)];
}

const FWeaponAppearance* UWarframeGameInstance::GetWeaponAppearance(EWeaponID WeaponID)const
{
	return &WeaponAppearanceArray[static_cast<int32>(WeaponID)];
}

const FPickableObjectInfo* UWarframeGameInstance::GetPickableObjectInfo(EPickableObjectID PickableObjectID)const
{
	return &PickableObjectInfoArray[static_cast<int32>(PickableObjectID)];
}

void UWarframeGameInstance::ReadInDataTables()
{
	FString FilePath;
	const char *Begin;
	const char *End;

	// Read in character table.
	{
		FilePath = FPaths::ProjectContentDir() + TEXT("DataTable/CharacterTable.dt");

		if (Warframe::GetFileContent(FilePath, Begin, End))
		{
			this->ReadInCharacterTable(Begin, End);

			delete Begin;
		}
	}

	// Read in character appearance table.
	{
		FilePath = FPaths::ProjectContentDir() + TEXT("DataTable/CharacterAppearanceTable.dt");

		if (Warframe::GetFileContent(FilePath, Begin, End))
		{
			this->ReadInCharacterAppearanceTable(Begin, End);

			delete Begin;
		}
	}

	// Read in enemy table.
	{
		FilePath = FPaths::ProjectContentDir() + TEXT("DataTable/EnemyTable.dt");

		if (Warframe::GetFileContent(FilePath, Begin, End))
		{
			this->ReadInEnemyTable(Begin, End);

			delete Begin;
		}
	}

	// Read in warframe table.
	{
		FilePath = FPaths::ProjectContentDir() + TEXT("DataTable/WarframeTable.dt");

		if (Warframe::GetFileContent(FilePath, Begin, End))
		{
			this->ReadInWarframeTable(Begin, End);

			delete Begin;
		}
	}

	// Read in weapon table.
	{
		FilePath = FPaths::ProjectContentDir() + TEXT("DataTable/WeaponTable.dt");

		if (Warframe::GetFileContent(FilePath, Begin, End))
		{
			this->ReadInWeaponTable(Begin, End);

			delete Begin;
		}
	}

	// Read in weapon appearance table.
	{
		FilePath = FPaths::ProjectContentDir() + TEXT("DataTable/WeaponAppearanceTable.dt");

		if (Warframe::GetFileContent(FilePath, Begin, End))
		{
			this->ReadInWeaponAppearanceTable(Begin, End);

			delete Begin;
		}
	}

	// Read in pickable object table.
	{
		FilePath = FPaths::ProjectContentDir() + TEXT("DataTable/PickableObjectTable.dt");

		if (Warframe::GetFileContent(FilePath, Begin, End))
		{
			this->ReadInPickableObjectTable(Begin, End);

			delete Begin;
		}
	}
}

void UWarframeGameInstance::ReadInCharacterTable(const char* Begin, const char* End)
{
	// Reserve space for all character info.
	CharacterInfoArray.SetNum(static_cast<int32>(CastToUnderlyingType(ECharacterID::End) - CastToUnderlyingType(ECharacterID::Begin) + 1));

	// Read in character name list.
	{
		uint32 NameCount;
		NameCount = *reinterpret_cast<const uint32*>(Begin);
		Begin += sizeof(NameCount);

		uint32 WordLength;

		for (uint32 i = 0; i < NameCount; ++i)
		{
			FName NewName = Warframe::GetWord(Begin, '\0', WordLength);
			CharacterInfoArray[i].Name = NewName;
			Begin += WordLength + 1;
		}
	}

	// Read in character info.
	ECharacterID CharacterID;

	while (Begin != End)
	{
		CharacterID = *reinterpret_cast<const ECharacterID*>(Begin);
		Begin += sizeof(CharacterID);

		FCharacterInfo& NewCharacterInfo = CharacterInfoArray[static_cast<int32>(CharacterID)];

		Warframe::ReadIn(NewCharacterInfo.BaseLevel, Begin);
		Warframe::ReadIn(NewCharacterInfo.HealthType, Begin);
		Warframe::ReadIn(NewCharacterInfo.Health, Begin);
		Warframe::ReadIn(NewCharacterInfo.ShieldType, Begin);
		Warframe::ReadIn(NewCharacterInfo.Shield, Begin);
		Warframe::ReadIn(NewCharacterInfo.ArmorType, Begin);
		Warframe::ReadIn(NewCharacterInfo.Armor, Begin);
		Warframe::ReadIn(NewCharacterInfo.Affinity, Begin);
	}
}

void UWarframeGameInstance::ReadInCharacterAppearanceTable(const char* Begin, const char* End)
{
	// Reserve space for all character appearances.
	CharacterAppearanceArray.SetNum(static_cast<int32>(CastToUnderlyingType(ECharacterID::End) - CastToUnderlyingType(ECharacterID::Begin) + 1));

	TArray<FName> MeshArray;

	// Read in mesh list.
	{
		uint32 MeshCount;
		MeshCount = *reinterpret_cast<const uint32*>(Begin);
		Begin += sizeof(MeshCount);

		MeshArray.SetNum(MeshCount);

		uint32 WordLength;

		for (uint32 i = 0; i < MeshCount; ++i)
		{
			FName NewMesh = Warframe::GetWord(Begin, '\0', WordLength);
			MeshArray[i] = NewMesh;
			Begin += WordLength + 1;
		}
	}

	TArray<FName> AnimClassArray;

	// Read in anim class list.
	{
		uint32 AnimClassCount;
		AnimClassCount = *reinterpret_cast<const uint32*>(Begin);
		Begin += sizeof(AnimClassCount);

		AnimClassArray.SetNum(AnimClassCount);

		uint32 WordLength;

		for (uint32 i = 0; i < AnimClassCount; ++i)
		{
			FName NewAnimClass = Warframe::GetWord(Begin, '\0', WordLength);
			AnimClassArray[i] = NewAnimClass;
			Begin += WordLength + 1;
		}
	}

	// Read in character appearances.
	{
		ECharacterID CharacterID;
		uint32 tempUint32;

		while (Begin != End)
		{
			CharacterID = *reinterpret_cast<const ECharacterID*>(Begin);
			Begin += sizeof(CharacterID);

			FCharacterAppearance& NewAppearance = CharacterAppearanceArray[static_cast<int32>(CharacterID)];

			Warframe::ReadIn(tempUint32, Begin);
			NewAppearance.Mesh = MeshArray[tempUint32];
			Warframe::ReadIn(tempUint32, Begin);
			NewAppearance.AnimClass = AnimClassArray[tempUint32];
			Warframe::ReadIn(NewAppearance.RelativeLocation.X, Begin);
			Warframe::ReadIn(NewAppearance.RelativeLocation.Y, Begin);
			Warframe::ReadIn(NewAppearance.RelativeLocation.Z, Begin);
			Warframe::ReadIn(NewAppearance.RelativeRotation.Roll, Begin);
			Warframe::ReadIn(NewAppearance.RelativeRotation.Pitch, Begin);
			Warframe::ReadIn(NewAppearance.RelativeRotation.Yaw, Begin);
			Warframe::ReadIn(NewAppearance.HalfHeight, Begin);
			Warframe::ReadIn(NewAppearance.Radius, Begin);
		}
	}
}

void UWarframeGameInstance::ReadInEnemyTable(const char* Begin, const char* End)
{
	// Reserve space for all enemy info
	EnemyInfoArray.SetNum(static_cast<int32>(CastToUnderlyingType(ECharacterID::Enemy_End) - CastToUnderlyingType(ECharacterID::Begin) + 1));

	// Read in warframe info.
	ECharacterID EnemyID;

	while (Begin != End)
	{
		EnemyID = *reinterpret_cast<const ECharacterID*>(Begin);
		Begin += sizeof(EnemyID);

		FEnemyInfo& NewEnemyInfo = EnemyInfoArray[static_cast<int32>(EnemyID)];

		Warframe::ReadIn(NewEnemyInfo.Faction, Begin);
		Warframe::ReadIn(NewEnemyInfo.Weapon1, Begin);
		Warframe::ReadIn(NewEnemyInfo.Weapon2, Begin);
	}
}

void UWarframeGameInstance::ReadInWarframeTable(const char* Begin, const char* End)
{
	// Reserve space for all warframe info
	WarframeInfoArray.SetNum(static_cast<int32>(CastToUnderlyingType(ECharacterID::End) - CastToUnderlyingType(ECharacterID::Enemy_End)));

	// Read in warframe info.
	ECharacterID CharacterID;

	while (Begin != End)
	{
		CharacterID = *reinterpret_cast<const ECharacterID*>(Begin);
		Begin += sizeof(CharacterID);

		int32 WarframeID = CastToUnderlyingType(CharacterID) - CastToUnderlyingType(ECharacterID::Enemy_End) - 1;

		FWarframeInfo& NewWarframeInfo = WarframeInfoArray[static_cast<int32>(WarframeID)];

		Warframe::ReadIn(NewWarframeInfo.Energy, Begin);
		Warframe::ReadIn(NewWarframeInfo.SprintSpeed, Begin);
		Warframe::ReadIn(NewWarframeInfo.AuraPolarity, Begin);
		Warframe::ReadIn(NewWarframeInfo.ExilusPolarity, Begin);
		Warframe::ReadIn(NewWarframeInfo.Polarity1, Begin);
		Warframe::ReadIn(NewWarframeInfo.Polarity2, Begin);
		Warframe::ReadIn(NewWarframeInfo.Polarity3, Begin);
		Warframe::ReadIn(NewWarframeInfo.Polarity4, Begin);
		Warframe::ReadIn(NewWarframeInfo.Polarity5, Begin);
		Warframe::ReadIn(NewWarframeInfo.Polarity6, Begin);
		Warframe::ReadIn(NewWarframeInfo.Polarity7, Begin);
		Warframe::ReadIn(NewWarframeInfo.Polarity8, Begin);
	}
}

void UWarframeGameInstance::ReadInWeaponTable(const char* Begin, const char* End)
{
	// Reserve space for all weapon info.
	WeaponInfoArray.SetNum(static_cast<int32>(CastToUnderlyingType(EWeaponID::End) - CastToUnderlyingType(EWeaponID::Begin) + 1));
	
	// Read in weapon name list.
	{
		uint32 NameCount;
		NameCount = *reinterpret_cast<const uint32*>(Begin);
		Begin += sizeof(NameCount);

		uint32 WordLength;

		for (uint32 i = 0; i < NameCount; ++i)
		{
			FName NewName = Warframe::GetWord(Begin, '\0', WordLength);
			WeaponInfoArray[i].Name = NewName;
			Begin += WordLength + 1;
		}
	}

	// Read in weapon info.
	EWeaponID WeaponID;

	while (Begin != End)
	{
		WeaponID = *reinterpret_cast<const EWeaponID*>(Begin);
		Begin += sizeof(WeaponID);

		FWeaponInfo &WeaponInfo = WeaponInfoArray[static_cast<int32>(WeaponID)];

		Warframe::ReadIn(WeaponInfo.AmmoType, Begin);
		Warframe::ReadIn(WeaponInfo.Magazine, Begin);
		Warframe::ReadIn(WeaponInfo.Ammo, Begin);

		TArray<FWeaponModeInfo> &ModeInfoArray = WeaponInfo.ModeInfoArray;
		ModeInfoArray.SetNum(ModeInfoArray.Num() + 1);
		FWeaponModeInfo &CurrentWeaponMode = ModeInfoArray[ModeInfoArray.Num() - 1];

		Warframe::ReadIn(CurrentWeaponMode.Accuracy, Begin);
		Warframe::ReadIn(CurrentWeaponMode.ChargeRate, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Pellets, Begin);
		Warframe::ReadIn(CurrentWeaponMode.CriticalChance, Begin);
		Warframe::ReadIn(CurrentWeaponMode.CriticalMultiplier, Begin);
		Warframe::ReadIn(CurrentWeaponMode.FalloffStart, Begin);
		Warframe::ReadIn(CurrentWeaponMode.FalloffEnd, Begin);
		Warframe::ReadIn(CurrentWeaponMode.FalloffDamage, Begin);
		Warframe::ReadIn(CurrentWeaponMode.FireRate, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Noise, Begin);
		Warframe::ReadIn(CurrentWeaponMode.PunchThrough, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Reload, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Status, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Trigger, Begin);
		Warframe::ReadIn(CurrentWeaponMode.MaxBurstCount, Begin);
		Warframe::ReadIn(CurrentWeaponMode.MaxChargeRounds, Begin);
		Warframe::ReadIn(CurrentWeaponMode.ChargeMultiplier, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Slash, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Impact, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Puncture, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Heat, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Cold, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Electricity, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Toxin, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Blast, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Radiation, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Gas, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Magnetic, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Viral, Begin);
		Warframe::ReadIn(CurrentWeaponMode.Corrosive, Begin);
	}
}

void UWarframeGameInstance::ReadInWeaponAppearanceTable(const char* Begin, const char* End)
{	
	// Reserve space for all weapon appearances.
	WeaponAppearanceArray.SetNum(static_cast<int32>(CastToUnderlyingType(EWeaponID::End) - CastToUnderlyingType(EWeaponID::Begin) + 1));

	TArray<FName> MeshArray;

	// Read in mesh list.
	{
		uint32 MeshCount;
		MeshCount = *reinterpret_cast<const uint32*>(Begin);
		Begin += sizeof(MeshCount);

		MeshArray.SetNum(MeshCount);

		uint32 WordLength;

		for (uint32 i = 0; i < MeshCount; ++i)
		{
			FName NewMesh = Warframe::GetWord(Begin, '\0', WordLength);
			MeshArray[i] = NewMesh;
			Begin += WordLength + 1;
		}
	}

	TArray<FName> FireEmitterArray;

	// Read in fire emitter list.
	{
		uint32 FireEmitterCount;
		FireEmitterCount = *reinterpret_cast<const uint32*>(Begin);
		Begin += sizeof(FireEmitterCount);

		FireEmitterArray.SetNum(FireEmitterCount);

		uint32 WordLength;

		for (uint32 i = 0; i < FireEmitterCount; ++i)
		{
			FName NewFireEmitter = Warframe::GetWord(Begin, '\0', WordLength);
			FireEmitterArray[i] = NewFireEmitter;
			Begin += WordLength + 1;
		}
	}

	TArray<FName> ReloadAnimArray;

	// Read in reload anim list.
	{
		uint32 ReloadAnimCount;
		ReloadAnimCount = *reinterpret_cast<const uint32*>(Begin);
		Begin += sizeof(ReloadAnimCount);

		ReloadAnimArray.SetNum(ReloadAnimCount);

		uint32 WordLength;

		for (uint32 i = 0; i < ReloadAnimCount; ++i)
		{
			FName NewReloadAnim = Warframe::GetWord(Begin, '\0', WordLength);
			ReloadAnimArray[i] = NewReloadAnim;
			Begin += WordLength + 1;
		}
	}

	TArray<FName> OnHitEmitterArray;

	// Read in on hit emitter list.
	{
		uint32 OnHitEmitterCount;
		OnHitEmitterCount = *reinterpret_cast<const uint32*>(Begin);
		Begin += sizeof(OnHitEmitterCount);

		OnHitEmitterArray.SetNum(OnHitEmitterCount);

		uint32 WordLength;

		for (uint32 i = 0; i < OnHitEmitterCount; ++i)
		{
			FName NewOnHitEmitter = Warframe::GetWord(Begin, '\0', WordLength);
			OnHitEmitterArray[i] = NewOnHitEmitter;
			Begin += WordLength + 1;
		}
	}

	// Read in weapon appearances.
	{
		EWeaponID WeaponID;
		uint32 tempUint32;

		while (Begin != End)
		{
			WeaponID = *reinterpret_cast<const EWeaponID*>(Begin);
			Begin += sizeof(WeaponID);

			FWeaponAppearance& NewAppearance = WeaponAppearanceArray[static_cast<int32>(WeaponID)];

			Warframe::ReadIn(tempUint32, Begin);
			NewAppearance.Mesh = MeshArray[tempUint32];
			Warframe::ReadIn(tempUint32, Begin);
			NewAppearance.FireEmitter = FireEmitterArray[tempUint32];
			Warframe::ReadIn(tempUint32, Begin);
			NewAppearance.ReloadAnim = ReloadAnimArray[tempUint32];
			Warframe::ReadIn(tempUint32, Begin);
			NewAppearance.OnHitEmitter = OnHitEmitterArray[tempUint32];
		}
	}
}

void UWarframeGameInstance::ReadInPickableObjectTable(const char* Begin, const char* End)
{
	// Reserve space for all pickable objects.
	PickableObjectInfoArray.SetNum(static_cast<int32>(CastToUnderlyingType(EPickableObjectID::End) - CastToUnderlyingType(EPickableObjectID::Begin) + 1));

	TArray<FName> MeshArray;

	// Read in mesh list.
	{
		uint32 MeshCount;
		MeshCount = *reinterpret_cast<const uint32*>(Begin);
		Begin += sizeof(MeshCount);

		MeshArray.SetNum(MeshCount);

		uint32 WordLength;

		for (uint32 i = 0; i < MeshCount; ++i)
		{
			FName NewMesh = Warframe::GetWord(Begin, '\0', WordLength);
			MeshArray[i] = NewMesh;
			Begin += WordLength + 1;
		}
	}

	// Read in pickable objects.
	{
		EPickableObjectID PickableObjectID;
		EPredefinedColorID PredefinedColorID;
		uint32 tempUint32;

		while (Begin != End)
		{
			PickableObjectID = *reinterpret_cast<const EPickableObjectID*>(Begin);
			Begin += sizeof(PickableObjectID);

			PredefinedColorID = *reinterpret_cast<const EPredefinedColorID*>(Begin);
			Begin += sizeof(PredefinedColorID);

			FPickableObjectInfo& NewPickableObjectInfo = PickableObjectInfoArray[static_cast<int32>(PickableObjectID)];

			switch (PredefinedColorID)
			{
			case EPredefinedColorID::White:
				NewPickableObjectInfo.BeamColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			case EPredefinedColorID::Black:
				NewPickableObjectInfo.BeamColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
				break;
			case EPredefinedColorID::Transparent:
				NewPickableObjectInfo.BeamColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
				break;
			case EPredefinedColorID::Red:
				NewPickableObjectInfo.BeamColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
				break;
			case EPredefinedColorID::Green:
				NewPickableObjectInfo.BeamColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
				break;
			case EPredefinedColorID::Blue:
				NewPickableObjectInfo.BeamColor = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f);
				break;
			case EPredefinedColorID::Yellow:
				NewPickableObjectInfo.BeamColor = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f);
				break;
			case EPredefinedColorID::Cyan:
				NewPickableObjectInfo.BeamColor = FLinearColor(0.0f, 1.0f, 1.0f, 1.0f);
				break;
			case EPredefinedColorID::Magenta:
				NewPickableObjectInfo.BeamColor = FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);
				break;
			case EPredefinedColorID::Orange:
				NewPickableObjectInfo.BeamColor = FLinearColor(1.0f, 0.5f, 0.0f, 1.0f);
				break;
			case EPredefinedColorID::Purple:
				NewPickableObjectInfo.BeamColor = FLinearColor(0.5f, 0.0f, 0.5f, 1.0f);
				break;
			case EPredefinedColorID::Turquoise:
				NewPickableObjectInfo.BeamColor = FLinearColor(0.698039f, 1.0f, 1.0f, 1.0f);
				break;
			case EPredefinedColorID::Silver:
				NewPickableObjectInfo.BeamColor = FLinearColor(0.75, 0.75f, 0.75f, 1.0f);
				break;
			case EPredefinedColorID::Emerald:
				NewPickableObjectInfo.BeamColor = FLinearColor(0.25f, 0.88f, 0.815686f, 1.0f);
				break;
			default:
				NewPickableObjectInfo.BeamColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
				break;
			}
			Warframe::ReadIn(tempUint32, Begin);
			NewPickableObjectInfo.Mesh = MeshArray[tempUint32];
		}
	}
}

void UWarframeGameInstance::InitFactoryClassOverrides()
{
	FString JsonString;
	{
		const char* Begin;
		const char* End;
		if (Warframe::GetFileContent(FPaths::ProjectContentDir() + TEXT("Config/ClassOverrides.json"), Begin, End))
		{
			JsonString = FString(Begin);
			delete Begin;
		}
	}

	if (JsonString.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("FWeaponFactory failed to read config file."));
		return;
	}

	TSharedPtr<FJsonObject> JsonRoot;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	if (FJsonSerializer::Deserialize(JsonReader, JsonRoot))
	{
		this->InitCharacterFactoryClassOverrides(JsonRoot);

		this->InitWeaponFactoryClassOverrides(JsonRoot);
	}
}

void UWarframeGameInstance::InitCharacterFactoryClassOverrides(TSharedPtr<FJsonObject> &JsonRoot)
{
	FCharacterFactory::Instance().ClearOverrides();

	TMap<FName, ECharacterID> CharacterNameIDMap;
	for (UnderlyingType<ECharacterID> i = CastToUnderlyingType(ECharacterID::Begin); i <= CastToUnderlyingType(ECharacterID::End); ++i)
	{
		CharacterNameIDMap.Add(CharacterInfoArray[i].Name, static_cast<ECharacterID>(i));
	}

	auto& Characters = JsonRoot->GetArrayField("Characters");
	for (int32 i = 0; i < Characters.Num(); ++i)
	{
		auto& Character = Characters[i]->AsObject();

		FName CharacterName = *Character->GetStringField("Name");

		FString CharacterBPClass = *Character->GetStringField("BPClass");

		ECharacterID* Result = CharacterNameIDMap.Find(CharacterName);
		if (Result != nullptr)
		{
			UClass* ClassOverride = LoadObject<UClass>(nullptr, *CharacterBPClass);
			if (ClassOverride != nullptr)
			{
				FCharacterFactory::Instance().SetOverride(*Result, ClassOverride);
			}
		}
	}
}

void UWarframeGameInstance::InitWeaponFactoryClassOverrides(TSharedPtr<FJsonObject> &JsonRoot)
{
	FWeaponFactory::Instance().ClearOverrides();

	TMap<FName, EWeaponID> WeaponNameIDMap;
	for (UnderlyingType<EWeaponID> i = CastToUnderlyingType(EWeaponID::Begin); i <= CastToUnderlyingType(EWeaponID::End); ++i)
	{
		WeaponNameIDMap.Add(WeaponInfoArray[i].Name, static_cast<EWeaponID>(i));
	}

	auto& Weapons = JsonRoot->GetArrayField("Weapons");
	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		auto& Weapon = Weapons[i]->AsObject();

		FName WeaponName = *Weapon->GetStringField("Name");

		FString WeaponBPClass = *Weapon->GetStringField("BPClass");

		EWeaponID* Result = WeaponNameIDMap.Find(WeaponName);
		if (Result != nullptr)
		{
			UClass* ClassOverride = LoadObject<UClass>(nullptr, *WeaponBPClass);
			if (ClassOverride != nullptr)
			{
				FWeaponFactory::Instance().SetOverride(*Result, ClassOverride);
			}
		}
	}
}
