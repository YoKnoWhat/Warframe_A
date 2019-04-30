
#pragma once

#include "UI/Mission/MissionPanel.h"
#include "Gameplay/GameMode/WarframeGameMode.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "MissionPanelExterminate.generated.h"


UCLASS()
class UMissionPanelExterminate : public UMissionPanel
{
	GENERATED_BODY()

public:
	virtual void Update(class AWarframe* Player)override;

protected:
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* CurrentCounter;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* TargetCounter;
};
