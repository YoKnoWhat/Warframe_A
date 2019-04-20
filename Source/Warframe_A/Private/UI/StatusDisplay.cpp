
#include "UI/StatusDisplay.h"
#include "Character/WarframeCharacter.h"


UStatusDisplay::UStatusDisplay(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{}

void UStatusDisplay::RefreshStatuses(AWarframeCharacter* Character)
{
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.AlwaysSign = false;
	NumberFormatOptions.UseGrouping = false;
	NumberFormatOptions.RoundingMode = ERoundingMode::FromZero;
	NumberFormatOptions.MinimumIntegralDigits = 1;
	NumberFormatOptions.MaximumIntegralDigits = 324;
	NumberFormatOptions.MinimumFractionalDigits = 1;
	NumberFormatOptions.MaximumFractionalDigits = 1;

	float StatusTime;

	StatusTime = Character->GetStatusTime(EDamageType::Slash);
	if (StatusTime == 0.0f)
	{
		this->SlashPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->SlashPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->SlashText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Impact);
	if (StatusTime == 0.0f)
	{
		this->ImpactPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->ImpactPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->ImpactText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Puncture);
	if (StatusTime == 0.0f)
	{
		this->PuncturePanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->PuncturePanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->PunctureText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Heat);
	if (StatusTime == 0.0f)
	{
		this->HeatPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->HeatPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->HeatText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Cold);
	if (StatusTime == 0.0f)
	{
		this->ColdPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->ColdPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->ColdText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Electricity);
	if (StatusTime == 0.0f)
	{
		this->ElectricityPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->ElectricityPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->ElectricityText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Toxin);
	if (StatusTime == 0.0f)
	{
		this->ToxinPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->ToxinPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->ToxinText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Blast);
	if (StatusTime == 0.0f)
	{
		this->BlastPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->BlastPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->BlastText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Radiation);
	if (StatusTime == 0.0f)
	{
		this->RadiationPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->RadiationPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->RadiationText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Gas);
	if (StatusTime == 0.0f)
	{
		this->GasPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->GasPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->GasText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Magnetic);
	if (StatusTime == 0.0f)
	{
		this->MagneticPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->MagneticPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->MagneticText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Viral);
	if (StatusTime == 0.0f)
	{
		this->ViralPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->ViralPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->ViralText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Corrosive);
	if (StatusTime == 0.0f)
	{
		this->CorrosivePanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->CorrosivePanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->CorrosiveText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}

	StatusTime = Character->GetStatusTime(EDamageType::Void);
	if (StatusTime == 0.0f)
	{
		this->VoidPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->VoidPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
		this->VoidText->SetText(FText::AsNumber(StatusTime, &NumberFormatOptions));
	}
}
