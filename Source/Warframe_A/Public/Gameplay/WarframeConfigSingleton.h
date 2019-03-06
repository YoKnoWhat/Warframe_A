#pragma once

#include "WarframeCommon.h"


/**
 *
 */
struct FWarframeConfigSingleton
{
public:
	static FWarframeConfigSingleton& Instance();

	void LoadConfig();

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

protected:
	struct FResourceRef
	{
		FName Type;
		FString Ref;
	};
	TMap<FName, FResourceRef> ResourcesMap; // Use FName to find specified resource object.
};
