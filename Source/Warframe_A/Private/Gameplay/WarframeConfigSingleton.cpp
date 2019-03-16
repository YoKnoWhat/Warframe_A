// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/WarframeConfigSingleton.h"
#include "UI/CharacterHeaderPanel.h"
#include "Utility/HelperFunction.h"

#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Core/Public/Misc/ConfigCacheIni.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "Runtime/Json/Public/Serialization/JsonReader.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"


FWarframeConfigSingleton& FWarframeConfigSingleton::Instance()
{
	static FWarframeConfigSingleton Inst;
	return Inst;
}

void FWarframeConfigSingleton::LoadConfig()
{
	FString JsonString;
	{
		const char* Begin;
		const char* End;
		if (Warframe::GetFileContent(FPaths::ProjectContentDir() + TEXT("Config/Resources.json"), Begin, End))
		{
			JsonString = FString(Begin);
			delete Begin;
		}
	}

	if (JsonString.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("UWarframeConfigSingleton failed to read config file."));
		return;
	}

	TSharedPtr<FJsonObject> JsonRoot;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	if (FJsonSerializer::Deserialize(JsonReader, JsonRoot))
	{
		auto& ResourceRefs = JsonRoot->GetArrayField("ResourceRefs");
		for (int32 i = 0; i < ResourceRefs.Num(); ++i)
		{
			auto& ResourceRef = ResourceRefs[i]->AsObject();

			FString ResType = ResourceRef->GetStringField("Type");

			FString ResStr = ResourceRef->GetStringField("Ref");
			FString ResName = Warframe::GetWord(ResStr, 0, TEXT('='));
			FString ResRef = ResStr.Mid(ResName.Len() + 1);

			ResourcesMap.Add(*ResName, { *ResType, ResRef });
		}

		auto& ClassRefs = JsonRoot->GetArrayField("ClassRefs");
		for (int32 i = 0; i < ClassRefs.Num(); ++i)
		{
			auto& ClassRef = ClassRefs[i]->AsObject();

			FString ClsType = ClassRef->GetStringField("Type");

			FString ClsStr = ClassRef->GetStringField("Ref");
			FString ClsName = Warframe::GetWord(ClsStr, 0, TEXT('='));
			FString ClsRef = ClsStr.Mid(ClsName.Len() + 1);

			ClassesMap.Add(*ClsName, { *ClsType, ClsRef });
		}
	}
}

UClass* FWarframeConfigSingleton::FindResourceClass(FName Name)
{
	UClass* Class;

	auto Result = ClassesMap.Find(Name);
	if (Result != nullptr)
	{
		Class = LoadObject<UClass>(nullptr, *Result->Ref);

		if (Class == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWarframeConfigSingleton failed to load %s."), *Name.ToString());
			Class = Warframe::GetNativeClassByName(Result->Type);
		}
	}
	else
	{
		Class = nullptr;
	}

	return Class;
}

FWarframeConfigSingleton::FWarframeConfigSingleton()
{
	this->LoadConfig();
}

UObject* FWarframeConfigSingleton::GetResourceDefaultObject(FName Name)
{
	auto Result = ResourcesMap.Find(Name);
	if (Result != nullptr)
	{
		UClass* Class = Warframe::GetNativeClassByName(Result->Type);
		if (Class != nullptr)
		{
			return Class->GetDefaultObject();
		}
	}
	return nullptr;
}
