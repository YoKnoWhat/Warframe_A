
#pragma once

#ifdef UE_BUILD_DEVELOPMENT
#include "CoreMinimal.h"
#endif


/**
 * The numbers of call to Get() and Put() methods must match, otherwise there will be memory leak.
 */
template<class T>
class FObjectPool
{
private:
	struct FInternalWrapper : public T
	{
		// Add an underscore to variable name to avoid potential naming conflict.
		FInternalWrapper* Next_;
	};

public:
	FObjectPool() :
		Available(nullptr)
	{}

	~FObjectPool()
	{
		this->Clear();
	}

	T* Get()
	{
#ifdef UE_BUILD_DEVELOPMENT
		++Counter;
#endif
		if (Available == nullptr)
		{
			return new FInternalWrapper;
		}
		else
		{
			FInternalWrapper* Result = Available;
			Available = Available->Next_;
			return Result;
		}
	}

	void Put(T* Object)
	{
#ifdef UE_BUILD_DEVELOPMENT
		--Counter;
#endif
		FInternalWrapper* NewAvailable = static_cast<FInternalWrapper*>(Object);
		NewAvailable->~FInternalWrapper();
		NewAvailable->Next_ = Available;
		Available = NewAvailable;
	}

	void Clear()
	{
#ifdef UE_BUILD_DEVELOPMENT
		if (Counter != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Object pool memory LEAK [%d]!"), Counter);
		}
#endif

		while (Available != nullptr)
		{
			FInternalWrapper* Next = Available->Next_;
			delete Available;
			Available = Next;
		}
	}

private:
	FInternalWrapper* Available;
#ifdef UE_BUILD_DEVELOPMENT
	uint32 Counter;
#endif
};
