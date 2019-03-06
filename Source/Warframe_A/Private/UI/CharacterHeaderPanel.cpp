// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CharacterHeaderPanel.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/WarframeConfigSingleton.h"

#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"
#include "Runtime/UMG/Public/Components/CanvasPanelSlot.h"
#include "Runtime/UMG/Public/Components/Overlay.h"
#include "Runtime/UMG/Public/Components/OverlaySlot.h"
#include "Runtime/UMG/Public/Components/WrapBox.h"


#define HEALTH_LINEAR_COLOR FLinearColor(0.612f, 0.018f, 0.018f, 1.0f)
#define SHIELD_LINEAR_COLOR FLinearColor(0.0f, 0.673f, 1.0f, 1.0f)
#define ARMOR_LINEAR_COLOR FLinearColor(0.737f, 0.389f, 0.031f, 1.0f)

TArray<UCharacterHeaderPanel*> UCharacterHeaderPanel::VisibleWidgetArray;

UCharacterHeaderPanel::UCharacterHeaderPanel(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	this->Visibility = ESlateVisibility::Hidden;
}

void UCharacterHeaderPanel::BeginDestroy()
{
	Super::BeginDestroy();

	// Game instance might be nullptr when shutting down.
	UGameInstance* GameInstance = this->GetGameInstance();
	if (GameInstance != nullptr)
	{
		if (FadeOutTimer.IsValid())
		{
			GameInstance->GetTimerManager().ClearTimer(FadeOutTimer);
		}
		if (HealthDropTimer.IsValid())
		{
			GameInstance->GetTimerManager().ClearTimer(HealthDropTimer);
		}
		if (ShieldDropTimer.IsValid())
		{
			GameInstance->GetTimerManager().ClearTimer(ShieldDropTimer);
		}
	}

	VisibleWidgetArray.Remove(this);
}

void UCharacterHeaderPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update header bar.
	this->UpdateHeaderBar();

	// Show/Hide status images.
	this->UpdateStatusImages();
}

void UCharacterHeaderPanel::Init()
{
	FWarframeConfigSingleton& ConfigInst = FWarframeConfigSingleton::Instance();

	UCanvasPanel* CanvasPanel = NewObject<UCanvasPanel>(this);

	// Init header bar.
	UOverlay *HeaderBarOverlay = NewObject<UOverlay>(this);
	CanvasPanel->AddChildToCanvas(HeaderBarOverlay);
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(HeaderBarOverlay->Slot);
		PanelSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		PanelSlot->SetAutoSize(true);

		this->HeaderBar = NewObject<UProgressBar>(this);
		HeaderBarOverlay->AddChild(this->HeaderBar);
		{
			 FSlateBrush& BackgroundImage = this->HeaderBar->WidgetStyle.BackgroundImage;
			 BackgroundImage.SetResourceObject(ConfigInst.FindResource<UTexture2D>("T_HeaderBarMask"));
			 BackgroundImage.DrawAs = ESlateBrushDrawType::Image;

			 FSlateBrush& FillImage = this->HeaderBar->WidgetStyle.FillImage;
			 FillImage.SetResourceObject(ConfigInst.FindResource<UMaterial>("M_HeaderBarFill"));
			 FillImage.DrawAs = ESlateBrushDrawType::Image;

			 FSlateBrush& MarqueeImage = this->HeaderBar->WidgetStyle.MarqueeImage;
			 UTexture2D* BackgroundTexture = Cast<UTexture2D>(BackgroundImage.GetResourceObject());
			 MarqueeImage.ImageSize = FVector2D(BackgroundTexture->GetSizeX(), BackgroundTexture->GetSizeY());
		}

		UImage* HeaderBarCover = NewObject<UImage>(this);
		HeaderBarOverlay->AddChild(HeaderBarCover);
		HeaderBarCover->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_HeaderBarCover"), true);
	}

	// Hold dynamic material reference of the widget.
	{
		FSlateBrush& Brush = this->HeaderBar->WidgetStyle.FillImage;
		UObject* Resource = this->HeaderBar->WidgetStyle.FillImage.GetResourceObject();

		// If we already have a dynamic material, use it.
		if ((DynamicMaterial = Cast<UMaterialInstanceDynamic>(Resource)) == nullptr)
		{
			// If the resource has a material interface we'll just update the brush to have a dynamic material.
			UMaterialInterface* Material = Cast<UMaterialInterface>(Resource);

			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, nullptr);
			Brush.SetResourceObject(DynamicMaterial);
		}
	}

	// Init header bar drops.
	this->HealthDrop = NewObject<UImage>(this);
	CanvasPanel->AddChildToCanvas(this->HealthDrop);
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(this->HealthDrop->Slot);
		PanelSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		PanelSlot->SetAlignment(FVector2D(0.0f, 0.5f));
		PanelSlot->SetAutoSize(true);

		this->HealthDrop->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_HeaderBarDropMask"), true);
		this->HealthDrop->SetColorAndOpacity(HEALTH_LINEAR_COLOR);
		this->HealthDrop->SetRenderTransformPivot(FVector2D(0.0f, 0.5f));
		this->HealthDrop->SetVisibility(ESlateVisibility::Hidden);
	}
	this->ShieldDrop = NewObject<UImage>(this);
	CanvasPanel->AddChildToCanvas(this->ShieldDrop);
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(this->ShieldDrop->Slot);
		PanelSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		PanelSlot->SetAlignment(FVector2D(0.0f, 0.5f));
		PanelSlot->SetAutoSize(true);
		this->ShieldDrop->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_HeaderBarDropMask"), true);
		this->ShieldDrop->SetColorAndOpacity(SHIELD_LINEAR_COLOR);
		this->ShieldDrop->SetRenderTransformPivot(FVector2D(0.0f, 0.5f));
		this->ShieldDrop->SetVisibility(ESlateVisibility::Hidden);
	}

	// Init name and status images.
	UWrapBox* NameStatusesWrapBox = NewObject<UWrapBox>(this);
	CanvasPanel->AddChildToCanvas(NameStatusesWrapBox);
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(NameStatusesWrapBox->Slot);
		PanelSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		PanelSlot->SetAlignment(FVector2D(0.5f, 1.0f));
		PanelSlot->SetAutoSize(true);

		this->Name = NewObject<UTextBlock>(this);
		this->Name->SetText(FText::FromName(this->WarframeCharacter->GetName()));
		this->Name->Font.Size = 10;
		this->Name->SetFont(this->Name->Font);
		this->Name->SetShadowOffset(FVector2D(1.0f, 0.5f));
		this->Name->SetShadowColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		NameStatusesWrapBox->AddChildWrapBox(this->Name);

		this->Slash = NewObject<UImage>(this);
		this->Slash->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Slash_Small"));
		this->Slash->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Slash->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Slash);

		this->Impact = NewObject<UImage>(this);
		this->Impact->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Impact_Small"));
		this->Impact->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Impact->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Impact);

		this->Puncture = NewObject<UImage>(this);
		this->Puncture->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Puncture_Small"));
		this->Puncture->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Puncture->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Puncture);

		this->Heat = NewObject<UImage>(this);
		this->Heat->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Heat_Small"));
		this->Heat->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Heat->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Heat);

		this->Cold = NewObject<UImage>(this);
		this->Cold->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Cold_Small"));
		this->Cold->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Cold->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Cold);

		this->Electricity = NewObject<UImage>(this);
		this->Electricity->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Electricity_Small"));
		this->Electricity->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Electricity->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Electricity);

		this->Toxin = NewObject<UImage>(this);
		this->Toxin->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Toxin_Small"));
		this->Toxin->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Toxin->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Toxin);

		this->Blast = NewObject<UImage>(this);
		this->Blast->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Blast_Small"));
		this->Blast->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Blast->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Blast);

		this->Radiation = NewObject<UImage>(this);
		this->Radiation->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Radiation_Small"));
		this->Radiation->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Radiation->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Radiation);

		this->Gas = NewObject<UImage>(this);
		this->Gas->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Gas_Small"));
		this->Gas->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Gas->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Gas);

		this->Magnetic = NewObject<UImage>(this);
		this->Magnetic->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Magnetic_Small"));
		this->Magnetic->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Magnetic->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Magnetic);

		this->Viral = NewObject<UImage>(this);
		this->Viral->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Viral_Small"));
		this->Viral->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Viral->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Viral);

		this->Corrosive = NewObject<UImage>(this);
		this->Corrosive->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Corrosive_Small"));
		this->Corrosive->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Corrosive->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Corrosive);

		this->Void = NewObject<UImage>(this);
		this->Void->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_Void_Small"));
		this->Void->SetBrushSize(FVector2D(12.0f, 12.0f));
		this->Void->SetVisibility(ESlateVisibility::Collapsed);
		NameStatusesWrapBox->AddChildWrapBox(this->Void);
	}

	// Init level text and level image.
	UOverlay* LevelOverlay = NewObject<UOverlay>(this);
	CanvasPanel->AddChildToCanvas(LevelOverlay);
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(LevelOverlay->Slot);
		PanelSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		PanelSlot->SetPosition(FVector2D(0.0f, 3.5f));
		PanelSlot->SetSize(FVector2D(114.0f, 20.0f));
		PanelSlot->SetAlignment(FVector2D(0.5f, 0.0f));

		this->Level = NewObject<UTextBlock>(this);
		LevelOverlay->AddChild(this->Level);
		{
			UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(this->Level->Slot);
			OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);

			this->Level->SetText(FText::AsNumber(this->WarframeCharacter->GetLevel(), nullptr));
			this->Level->Font.Size = 10;
			this->Level->SetFont(this->Level->Font);
			this->Level->SetShadowOffset(FVector2D(1.0f, 0.5f));
		}

		UImage* LevelImage = NewObject<UImage>(this);
		LevelOverlay->AddChild(LevelImage);
		{
			UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(LevelImage->Slot);
			OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);

			LevelImage->SetBrushFromTexture(ConfigInst.FindResource<UTexture2D>("T_LevelBackground"), true);
		}
	}
}

void UCharacterHeaderPanel::OnSelected()
{
	this->SetRenderOpacity(1.0f);

	if (FadeOutTimer.IsValid())
	{
		this->GetGameInstance()->GetTimerManager().ClearTimer(FadeOutTimer);
		VisibleWidgetArray.Remove(this);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (VisibleWidgetArray.Num() == 3)
		{
			VisibleWidgetArray[0]->OpacityStep = 1.0f;
			VisibleWidgetArray[0]->FadeOut();
		}
		else
		{
			for (int32 i = 0; i < VisibleWidgetArray.Num(); ++i)
			{
				VisibleWidgetArray[i]->OpacityStep *= 3.0f;
			}
		}
	}

	VisibleWidgetArray.Add(this);
}

void UCharacterHeaderPanel::OnUnselected()
{
	CurrentOpacity = 1.0f;
	OpacityStep = 1.0f / 60.0f;

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UCharacterHeaderPanel::FadeOut);

	this->GetGameInstance()->GetTimerManager().SetTimer(FadeOutTimer, Delegate, 1.0f / 60.0f, true);
}

void UCharacterHeaderPanel::FadeOut()
{
	CurrentOpacity -= OpacityStep;
	if (CurrentOpacity < 0.000001f)
	{
		this->GetGameInstance()->GetTimerManager().ClearTimer(FadeOutTimer);
		this->SetVisibility(ESlateVisibility::Hidden);
		VisibleWidgetArray.Remove(this);
	}
	else
	{
		this->SetRenderOpacity(CurrentOpacity);
	}
}

void UCharacterHeaderPanel::HealthDropFadeOut()
{
	HealthDropOpacity -= 1.0f / 15.0f;
	if (HealthDropOpacity < 0.000001f)
	{
		this->GetGameInstance()->GetTimerManager().ClearTimer(HealthDropTimer);
		this->HealthDrop->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		this->HealthDrop->SetRenderOpacity(HealthDropOpacity);
	}
}

void UCharacterHeaderPanel::ShieldDropFadeOut()
{
	ShieldDropOpacity -= 1.0f / 15.0f;
	if (ShieldDropOpacity < 0.000001f)
	{
		this->GetGameInstance()->GetTimerManager().ClearTimer(ShieldDropTimer);
		this->ShieldDrop->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		this->ShieldDrop->SetRenderOpacity(ShieldDropOpacity);
	}
}

void UCharacterHeaderPanel::UpdateHeaderBar()
{
	float InvHealthAndShield = 1.0f / (WarframeCharacter->GetMaxHealth() + WarframeCharacter->GetMaxShield());

	float NewHealthPercentCache = WarframeCharacter->GetCurrentHealth() * InvHealthAndShield;
	if (NewHealthPercentCache < HealthPercentCache)
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(this->HealthDrop->Slot);
		PanelSlot->SetPosition(FVector2D((NewHealthPercentCache - 0.5f) * 114.0f, 0.0f));

		HealthDropOpacity = 1.0f;
		this->HealthDrop->SetRenderOpacity(1.0f);
		this->HealthDrop->SetRenderScale(FVector2D(HealthPercentCache - NewHealthPercentCache, 1.0f));

		if (HealthDropTimer.IsValid() == false)
		{
			this->HealthDrop->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UCharacterHeaderPanel::HealthDropFadeOut);
		this->GetGameInstance()->GetTimerManager().SetTimer(HealthDropTimer, Delegate, 1.0f / 60.0f, true);
	}
	HealthPercentCache = NewHealthPercentCache;

	float NewShieldPercentCache = WarframeCharacter->GetCurrentShield() * InvHealthAndShield;
	if (NewShieldPercentCache < ShieldPercentCache)
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(this->ShieldDrop->Slot);
		PanelSlot->SetPosition(FVector2D((NewShieldPercentCache - 0.5f) * 114.0f, 0.0f));

		ShieldDropOpacity = 1.0f;
		this->ShieldDrop->SetRenderOpacity(1.0f);
		this->ShieldDrop->SetRenderScale(FVector2D(ShieldPercentCache - NewShieldPercentCache, 1.0f));

		if (ShieldDropTimer.IsValid() == false)
		{
			this->ShieldDrop->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UCharacterHeaderPanel::ShieldDropFadeOut);
		this->GetGameInstance()->GetTimerManager().SetTimer(ShieldDropTimer, Delegate, 1.0f / 60.0f, true);
	}
	ShieldPercentCache = NewShieldPercentCache;

	// Set scalar parameter values.
	DynamicMaterial->SetScalarParameterValue(FName("PercentA"), HealthPercentCache);
	DynamicMaterial->SetScalarParameterValue(FName("PercentB"), ShieldPercentCache);
}

void UCharacterHeaderPanel::UpdateStatusImages()
{
	if (WarframeCharacter->GetStatusTime(EDamageType::Slash) == 0.0f)
	{
		this->Slash->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Slash->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Impact) == 0.0f)
	{
		this->Impact->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Impact->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Puncture) == 0.0f)
	{
		this->Puncture->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Puncture->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Heat) == 0.0f)
	{
		this->Heat->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Heat->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Cold) == 0.0f)
	{
		this->Cold->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Cold->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Electricity) == 0.0f)
	{
		this->Electricity->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Electricity->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Toxin) == 0.0f)
	{
		this->Toxin->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Toxin->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Blast) == 0.0f)
	{
		this->Blast->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Blast->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Radiation) == 0.0f)
	{
		this->Radiation->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Radiation->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Gas) == 0.0f)
	{
		this->Gas->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Gas->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Magnetic) == 0.0f)
	{
		this->Magnetic->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Magnetic->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Viral) == 0.0f)
	{
		this->Viral->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Viral->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Corrosive) == 0.0f)
	{
		this->Corrosive->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Corrosive->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (WarframeCharacter->GetStatusTime(EDamageType::Void) == 0.0f)
	{
		this->Void->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->Void->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}
