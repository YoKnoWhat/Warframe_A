
#pragma once

#include "WarframeCommon.h"


struct FStateMachineLayer;

class WARFRAME_A_API FStateMachineLayerInitializer
{
	friend class UStateMachineComponent;

public:
	template<class T, class = std::enable_if_t<std::is_base_of<FStateMachineLayer, T>::value>>
	FStateMachineLayerInitializer& SetLayer(uint32 LayerIndex, T* Layer)
	{
		// todo: inheritance assert

		FStateMachineLayer** LayerPtr = LayerMap.Find(LayerIndex);
		if (LayerPtr == nullptr)
		{
			LayerMap.Add(LayerIndex, Layer);

			FMemory::Memset(Layer, 0, sizeof(T));
		}
		else
		{
			delete Layer;
		}

		return *this;
	}

	struct FStateMachineLayer* GetLayer(uint32 LayerIndex);

private:
	TMap<uint32, FStateMachineLayer*> LayerMap;
};
