// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DamagePopup.h"
#include "Gameplay/WarframeConfigSingleton.h"

#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/UMG/Public/Components/CanvasPanelSlot.h"
#include "Runtime/UMG/Public/Components/HorizontalBoxSlot.h"

void UDamagePopup::Init(UUserWidget* HUD)
{
	this->CachedHUD = HUD;

	// Init horizontal box.
	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(this->Slot);
	PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	PanelSlot->SetAnchors(FAnchors(0.0f, 0.0f, 0.0f, 0.0f));
	PanelSlot->SetAutoSize(true);
	
	this->SetVisibility(ESlateVisibility::Hidden);

	// Init damage text.
	this->DamageText = NewObject<UTextBlock>(this);
	this->AddChild(this->DamageText);
	this->DamageText->SetShadowOffset(FVector2D(1.0f, 3.0f));
	this->DamageText->SetShadowColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));

	FSlateFontInfo NewFont = this->DamageText->Font;
	NewFont.Size = 16;
	this->DamageText->SetFont(NewFont);

	// Init status image.
	this->StatusImage = NewObject<UImage>(this);
	this->AddChild(this->StatusImage);
	this->StatusImage->SetBrushSize(FVector2D(16.0f, 16.0f));

	UHorizontalBoxSlot* StatusImageSlot = Cast<UHorizontalBoxSlot>(this->StatusImage->Slot);
	StatusImageSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	StatusImageSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
}

void UDamagePopup::Show(const FVector &HitLocation_, EDamageType Status, float Damage, bool IsDamageOnShield, int32 CriticalTier)
{
	this->Time = 0.0f;
	this->HitLocation = HitLocation_;

	this->SetVisibility(ESlateVisibility::HitTestInvisible);

	bool ShouldStatusImageHidden = false;
	switch (Status)
	{
	case EDamageType::Slash:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Slash_Small"));
		break;
	case EDamageType::Impact:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Impact_Small"));
		break;
	case EDamageType::Puncture:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Puncture_Small"));
		break;
	case EDamageType::Heat:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Heat_Small"));
		break;
	case EDamageType::Cold:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Cold_Small"));
		break;
	case EDamageType::Electricity:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Electricity_Small"));
		break;
	case EDamageType::Toxin:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Toxin_Small"));
		break;
	case EDamageType::Blast:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Blast_Small"));
		break;
	case EDamageType::Radiation:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Radiation_Small"));
		break;
	case EDamageType::Gas:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Gas_Small"));
		break;
	case EDamageType::Magnetic:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Magnetic_Small"));
		break;
	case EDamageType::Viral:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Viral_Small"));
		break;
	case EDamageType::Corrosive:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Corrosive_Small"));
		break;
	case EDamageType::Void:
		this->StatusImage->SetBrushFromTexture(FWarframeConfigSingleton::Instance().FindResource<UTexture2D>("T_Void_Small"));
		break;
	default:
		this->StatusImage->SetVisibility(ESlateVisibility::Hidden);
		ShouldStatusImageHidden = true;
		break;
	}
	if (ShouldStatusImageHidden == false)
	{
		this->StatusImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.AlwaysSign = false;
	NumberFormatOptions.UseGrouping = true;
	NumberFormatOptions.RoundingMode = ERoundingMode::ToPositiveInfinity;
	NumberFormatOptions.MinimumIntegralDigits = 1;
	NumberFormatOptions.MaximumIntegralDigits = 324;
	NumberFormatOptions.MinimumFractionalDigits = 0;
	NumberFormatOptions.MaximumFractionalDigits = 0;
	this->DamageText->SetText(FText::AsNumber(Damage, &NumberFormatOptions));

	FLinearColor PopupColor;
	if (IsDamageOnShield)
	{
		PopupColor = FLinearColor(0.0f, 0.3f, 1.0f, 1.0f);
	}
	else if (CriticalTier == 0)
	{
		PopupColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (CriticalTier == 1)
	{
		PopupColor = FLinearColor(0.9f, 0.8f, 0.1f, 1.0f);
	}
	else if (CriticalTier == 2)
	{
		PopupColor = FLinearColor(1.0f, 0.1f, 0.0f, 1.0f);
	}
	else // if (CriticalTier >= 3)
	{
		PopupColor = FLinearColor(0.7f, 0.0f, 0.0f, 1.0f);
	}
	this->DamageText->SetColorAndOpacity(PopupColor);
	this->StatusImage->SetColorAndOpacity(PopupColor);

	if (CriticalTier == 0)
	{
		this->InitScale = 1.0f;
	}
	else
	{
		this->InitScale = 1.5f;
	}

	float RandomRadian = FMath::FRandRange(-45.0f/180.0f, 45.0f/180.0f);
	float NewMaxRandomY;
	float NewMaxRandomY1;
	{
		float k = /*(50.0f - 500.0f)*/-450.0f / (MaxRandomDist * MaxRandomDist - MinRandomDist * MinRandomDist);
		float k1 = /*(0.0f - 150.0f)*/-150.0f / (MaxRandomDist * MaxRandomDist - MinRandomDist * MinRandomDist);

		NewMaxRandomY = (CachedHUD->GetOwningPlayerPawn()->GetActorLocation() - HitLocation).SizeSquared() * k - k * MinRandomDist * MinRandomDist + 500.0f;
		NewMaxRandomY1 = (CachedHUD->GetOwningPlayerPawn()->GetActorLocation() - HitLocation).SizeSquared() * k1 - k1 * MinRandomDist * MinRandomDist + 150.0f;
		NewMaxRandomY = FMath::Clamp(NewMaxRandomY, 50.0f, 500.0f);
		NewMaxRandomY1 = FMath::Clamp(NewMaxRandomY1, 0.0f, 150.0f);
	}

	// Rotate (0.0, 1.0) by $RandomRadian.
	this->ControlPointOffset0 = FVector2D(FMath::Sin(RandomRadian), -FMath::Cos(RandomRadian)) * FMath::FRandRange(50.0f, NewMaxRandomY);
	this->ControlPointOffset1 = FVector2D(ControlPointOffset0.X * 2.0f, FMath::FRandRange(0.0f, NewMaxRandomY1));

	UGameViewportClient* ViewportClient = CachedHUD->GetWorld()->GetGameViewport();
	FVector2D ViewportSize;
	ViewportClient->GetViewportSize(ViewportSize);
	float ViewportScale = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));

	FVector2D ScreenLocation;
	CachedHUD->GetOwningPlayer()->ProjectWorldLocationToScreen(HitLocation, ScreenLocation);
	Cast<UCanvasPanelSlot>(this->Slot)->SetPosition(ScreenLocation / ViewportScale);
}

void UDamagePopup::Hide()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

bool UDamagePopup::Update(float DeltaTime)
{
	Time += DeltaTime;
	float OneMinusTime = 1.0f - Time;

	UGameViewportClient* ViewportClient = CachedHUD->GetWorld()->GetGameViewport();
	FVector2D ViewportSize;
	ViewportClient->GetViewportSize(ViewportSize);
	float ViewportScale = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));

	// Update position.
	FVector2D ScreenLocation;
	CachedHUD->GetOwningPlayer()->ProjectWorldLocationToScreen(HitLocation, ScreenLocation);
	ScreenLocation /= ViewportScale;

	Cast<UCanvasPanelSlot>(this->Slot)->SetPosition(OneMinusTime * OneMinusTime * ScreenLocation
		+ 2.0f * Time * OneMinusTime * (ScreenLocation + ControlPointOffset0)
		+ Time * Time *(ScreenLocation + ControlPointOffset1));

	// // Round the pixel projected value to reduce jittering due to layout rounding,
	// 		// I do this before I remove scaling, because scaling is going to be applied later
	// 		// in the opposite direction, so as long as we round, before inverse scale, scale should
	// 		// result in more or less the same value, especially after slate does layout rounding.
	// FVector2D ScreenPosition(FMath::RoundToInt(ScreenLocation.X), FMath::RoundToInt(ScreenLocation.Y));
	// 
	// FVector2D ViewportPosition;
	// USlateBlueprintLibrary::ScreenToViewport(CachedHUD->GetOwningPlayer(), ScreenPosition, ViewportPosition);

	// Apply scalar.
	float Scalar = InitScale;
	if (Time < 0.5f)
	{
		Scalar *= -0.5f * Time + 1.0f;
	}
	else
	{
		Scalar *= +0.5f * Time + 0.5f;
	}
	this->SetRenderScale(FVector2D(Scalar, Scalar));

	// Opacity.
	this->SetRenderOpacity(FMath::Min(2.0f * OneMinusTime, 1.0f));

	return Time > 1.0f;
}
