
#include "UI/Mission/MissionPanelExterminate.h"
#include "Character/Warframe/Warframe.h"
#include "Gameplay/GameMode/WarframeGameModeExterminate.h"

#include "Runtime/Engine/Classes/Engine/World.h"


void UMissionPanelExterminate::Update(AWarframe* Player)
{
	FNumberFormattingOptions IntegerFormatOptions;
	IntegerFormatOptions.AlwaysSign = false;
	IntegerFormatOptions.UseGrouping = false;
	IntegerFormatOptions.RoundingMode = ERoundingMode::HalfToEven;
	IntegerFormatOptions.MinimumIntegralDigits = 1;
	IntegerFormatOptions.MaximumIntegralDigits = 324;
	IntegerFormatOptions.MinimumFractionalDigits = 0;
	IntegerFormatOptions.MaximumFractionalDigits = 0;

	AWarframeGameModeExterminate* GameMode = Cast<AWarframeGameModeExterminate>(Player->GetWorld()->GetAuthGameMode());

	CurrentCounter->SetText(FText::AsNumber(GameMode->GetCurrentCounter(), &IntegerFormatOptions));
	TargetCounter->SetText(FText::AsNumber(GameMode->GetTargetCounter(), &IntegerFormatOptions));
}
