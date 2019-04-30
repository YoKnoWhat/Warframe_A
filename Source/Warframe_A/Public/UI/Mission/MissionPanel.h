
#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "MissionPanel.generated.h"


UCLASS()
class UMissionPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Update(class AWarframe* Player);
};
