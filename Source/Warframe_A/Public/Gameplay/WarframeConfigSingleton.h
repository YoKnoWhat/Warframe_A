#pragma once

#include "WarframeCommon.h"


/**
 * Global singleton for resource retrieve.
 * It can be used when GameInstance is not available.
 */
struct FWarframeConfigSingleton
{
public:
	static FWarframeConfigSingleton& Instance();

	void LoadConfig();

	UClass* FindResourceClass(FName Name);

	template<class T>
	T* FindResource(FName Name)
	{
		T* Resource;

		auto Result = ResourcesMap.Find(Name);
		if (Result != nullptr)
		{
			Resource = LoadObject<T>(nullptr, *Result->Ref);
		}
		else
		{
			Resource = nullptr;
		}

		if (Resource == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UWarframeConfigSingleton failed to load %s."), *Name.ToString());
			Resource = Cast<T>(this->GetResourceDefaultObject(Name));
			if (Resource == nullptr)
			{
				Resource = Cast<T>(T::StaticClass()->GetDefaultObject());
			}
		}
		return Resource;
	}

protected:
	FWarframeConfigSingleton();

	UObject* GetResourceDefaultObject(FName Name);

public:
	float FieldOfView = 90.0f;

protected:
	struct FObjectRef
	{
		FName Type;
		FString Ref;
	};
	TMap<FName, FObjectRef> ClassesMap; // Use FName to find specified resource object.
	TMap<FName, FObjectRef> ResourcesMap; // Use FName to find specified resource object.
};
