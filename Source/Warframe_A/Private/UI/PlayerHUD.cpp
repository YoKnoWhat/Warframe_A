
#include "UI/PlayerHUD.h"
#include "Character/Warframe/Warframe.h"
#include "Gameplay/GameMode/WarframeGameMode.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "UI/DamagePopup.h"
#include "UI/Minimap.h"
#include "UI/StatusDisplay.h"
#include "UI/Mission/MissionPanelExterminate.h"
#include "Weapon/WeaponBase.h"

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"


UPlayerHUD::UPlayerHUD(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	IntegerFormatOptions.AlwaysSign = false;
	IntegerFormatOptions.UseGrouping = false;
	IntegerFormatOptions.RoundingMode = ERoundingMode::HalfToEven;
	IntegerFormatOptions.MinimumIntegralDigits = 1;
	IntegerFormatOptions.MaximumIntegralDigits = 324;
	IntegerFormatOptions.MinimumFractionalDigits = 0;
	IntegerFormatOptions.MaximumFractionalDigits = 0;
}

void UPlayerHUD::Init(class AWarframe* InPlayer)
{
	Player = InPlayer;
}

void UPlayerHUD::OnDied()
{
}

void UPlayerHUD::OnApplyDamageToEnemy(FVector HitLocation, EDamageType StatusEffect, float Damage, bool IsDamageOnShield, int32 CriticalTier)
{
	if (DamageTextHead == (DamageTextTail + 1) % DamageTextCapacity)
	{
		DamageTextPool[DamageTextHead]->Hide();
		DamageTextHead = (DamageTextHead + 1) % DamageTextCapacity;
	}

	DamageTextPool[DamageTextTail]->Show(HitLocation, StatusEffect, Damage, IsDamageOnShield, CriticalTier);
	DamageTextTail = (DamageTextTail + 1) % DamageTextCapacity;
}

void UPlayerHUD::OnPlayerDamaged(bool IsDamageOnShield)
{
	// Update health & shield splash effect.
	if (IsDamageOnShield)
	{
		ShieldSplash->SetRenderOpacity(1.0f);

		if (ShieldSplashTimer.IsValid() == false)
		{
			ShieldSplash->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UPlayerHUD::ShieldSplashFadeOut);
		this->GetGameInstance()->GetTimerManager().SetTimer(ShieldSplashTimer, Delegate, 1.0f / 60.0f, true);
	}
	else
	{
		HealthSplash->SetRenderOpacity(1.0f);

		if (HealthSplashTimer.IsValid() == false)
		{
			HealthSplash->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UPlayerHUD::HealthSplashFadeOut);
		this->GetGameInstance()->GetTimerManager().SetTimer(HealthSplashTimer, Delegate, 1.0f / 60.0f, true);
	}
}

void UPlayerHUD::AddMission(EMissionType MissionType)
{
	FName MissionPanelClassName;

	switch (MissionType)
	{
	case EMissionType::Exterminate:
		MissionPanelClassName = "UI_MissionPanelExterminate";
		break;
	default:
		return;
	}

	MissionPanel->AddChildToVerticalBox(CreateWidget<UMissionPanel>(Player->GetGameInstance(), FWarframeConfigSingleton::Instance().FindResourceClass(MissionPanelClassName)));
}

void UPlayerHUD::RemoveMission(EMissionType MissionType)
{
	UClass* MissionPanelClass;

	switch (MissionType)
	{
	case EMissionType::Exterminate:
		MissionPanelClass = UMissionPanelExterminate::StaticClass();
		break;
	case EMissionType::Assassination:
	case EMissionType::Capture:
	case EMissionType::Defense:
	default:
		MissionPanelClass = nullptr;
		break;
	}

	for (int32 i = 0; i < MissionPanel->GetChildrenCount(); ++i)
	{
		if (MissionPanel->GetChildAt(i)->GetClass()->IsChildOf(MissionPanelClass))
		{
			MissionPanel->RemoveChildAt(i);
			break;
		}
	}
}

void UPlayerHUD::ClearMissions()
{
	MissionPanel->ClearChildren();
}

void UPlayerHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	this->InitDamageTextPool();

	Minimap->Init(Player->GetActorLocation(), 0.04267f);
}

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	this->UpdateHUDOffset();

	this->UpdateMiscWidgets();

	this->UpdateDamageTexts(DeltaTime);

	this->UpdateMissionPanel();

	Minimap->Update(Player);

	StatusDisplay->RefreshStatuses(Player);
}

void UPlayerHUD::NativeDestruct()
{
	Super::NativeDestruct();

	// Game instance might be nullptr when shutting down.
	UGameInstance* GameInstance = this->GetGameInstance();
	if (GameInstance != nullptr)
	{
		if (HealthSplashTimer.IsValid())
		{
			GameInstance->GetTimerManager().ClearTimer(HealthSplashTimer);
		}
		if (ShieldSplashTimer.IsValid())
		{
			GameInstance->GetTimerManager().ClearTimer(ShieldSplashTimer);
		}
	}
}

void UPlayerHUD::InitDamageTextPool()
{
	for (int32 i = 0; i < DamageTextCapacity; ++i)
	{
		UDamagePopup* NewDamagePopup = NewObject<UDamagePopup>(this);

		MainCanvasPanel->AddChildToCanvas(NewDamagePopup);
		NewDamagePopup->Init(this);
		DamageTextPool.Add(NewDamagePopup);
	}

	DamageTextHead = 0;
	DamageTextTail = 0;
}

void UPlayerHUD::UpdateHUDOffset()
{
	UCameraComponent* Camera = Cast<UCameraComponent>(Player->GetComponentByClass(UCameraComponent::StaticClass()));
	
	/** Camera should always be valid! */
	FVector PlayerVelocity = Player->GetVelocity();
	PlayerVelocity.Z = 0.0f;
	
	FRotator ViewRotator = Player->GetViewRotation();
	PlayerVelocity = ViewRotator.UnrotateVector(PlayerVelocity);

	MainCanvasPanel->SetRenderTranslation(FVector2D(PlayerVelocity.Y * (-1.0f / 30.0f), PlayerVelocity.Z * (1.0f / 20.0f)));
}

void UPlayerHUD::UpdateMiscWidgets()
{
	/** Player info panel. */
	PlayerShield->SetText(FText::AsNumber(Player->GetCurrentShield(), &IntegerFormatOptions));
	PlayerHealth->SetText(FText::AsNumber(Player->GetCurrentHealth(), &IntegerFormatOptions));
	PlayerShieldPercent->SetPercent(Player->GetCurrentShield() / Player->GetMaxShield());
	PlayerHealthPercent->SetPercent(Player->GetCurrentHealth() / Player->GetMaxHealth());
	PlayerName->SetText(FText::AsCultureInvariant(Player->GetName().ToString()));

	/** Companion info panel. */

	/** Weapon info panel. */
	WeaponMagazine->SetText(FText::AsNumber(Player->GetCurrentWeapon()->GetRemainingMagazine(), &IntegerFormatOptions));
	WeaponAmmo->SetText(FText::AsNumber(Player->GetCurrentWeapon()->GetRemainingAmmo(), &IntegerFormatOptions));
	WeaponName->SetText(FText::AsCultureInvariant(Player->GetCurrentWeapon()->GetName().ToString()));

	/** Ability info panel. */
	EnergyPercent->SetPercent(Player->GetCurrentEnergy() / Player->GetMaxEnergy());
	Energy->SetText(FText::AsNumber(Player->GetCurrentEnergy(), &IntegerFormatOptions));
}

void UPlayerHUD::UpdateDamageTexts(float DeltaTime)
{
#define DamageTextUpdateLoopBody() \
	DamagePopup = DamageTextPool[i]; \
	if (DamagePopup->Update(DeltaTime) == true) \
	{ \
		DamagePopup->Hide(); \
		DamageTextHead = (DamageTextHead + 1) % DamageTextCapacity; \
	}

	if (DamageTextHead != DamageTextTail)
	{
		UDamagePopup* DamagePopup;
		if (DamageTextHead > DamageTextTail)
		{
			for (int32 i = DamageTextHead; i < DamageTextCapacity; ++i)
			{
				DamageTextUpdateLoopBody();
			}

			if (DamageTextTail != 0)
			{
				for (int32 i = 0; i < DamageTextTail; ++i)
				{
					DamageTextUpdateLoopBody();
				}
			}
		}
		else
		{
			for (int32 i = DamageTextHead; i < DamageTextTail; ++i)
			{
				DamageTextUpdateLoopBody();
			}
		}
	}
#undef DamageTextUpdateLoopBody
}

void UPlayerHUD::UpdateMissionPanel()
{
	for (int32 i = 0; i < MissionPanel->GetChildrenCount(); ++i)
	{
		/** In case that blueprint override add non-mission-panel child to MissionPanel. */
		UMissionPanel* SpecMissionPanel = Cast<UMissionPanel>(MissionPanel->GetChildAt(i));
		if (SpecMissionPanel != nullptr)
		{
			SpecMissionPanel->Update(Player);
		}
	}
}

void UPlayerHUD::HealthSplashFadeOut()
{
	float NewOpacity = HealthSplash->GetRenderOpacity() - 1.0f / 15.0f;
	if (NewOpacity < 0.000001f)
	{
		this->GetGameInstance()->GetTimerManager().ClearTimer(HealthSplashTimer);
		HealthSplash->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HealthSplash->SetRenderOpacity(NewOpacity);
	}
}

void UPlayerHUD::ShieldSplashFadeOut()
{
	float NewOpacity = ShieldSplash->GetRenderOpacity() - 1.0f / 15.0f;
	if (NewOpacity < 0.000001f)
	{
		this->GetGameInstance()->GetTimerManager().ClearTimer(ShieldSplashTimer);
		ShieldSplash->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ShieldSplash->SetRenderOpacity(NewOpacity);
	}
}
