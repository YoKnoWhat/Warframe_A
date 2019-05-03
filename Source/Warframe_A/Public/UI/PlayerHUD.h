// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/GameMode/WarframeGameMode.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"
#include "Runtime/UMG/Public/Components/CanvasPanelSlot.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/VerticalBox.h"
#include "PlayerHUD.generated.h"


UCLASS()
class WARFRAME_A_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerHUD(const FObjectInitializer& ObjectInitializer);

	void Init(class AWarframe* InPlayer);

	void OnDied();

	void OnApplyDamageToEnemy(FVector HitLocation, EDamageType StatusEffect, float Damage, bool IsDamageOnShield, int32 CriticalTier);

	void OnPlayerDamaged(bool IsDamageOnShield);

	void AddMission(EMissionType MissionType);

	void RemoveMission(EMissionType MissionType);

	void ClearMissions();

protected:
	virtual void NativeOnInitialized()override;

	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime)override;

	virtual void NativeDestruct()override;

	void InitDamageTextPool();

	void UpdateHUDOffset();

	void UpdateMiscWidgets();

	void UpdateDamageTexts(float DeltaTime);

	void UpdateMissionPanel();

	void HealthSplashFadeOut();

	void ShieldSplashFadeOut();

protected:
	FNumberFormattingOptions IntegerFormatOptions;

	/** Only available after NativeOnInitialized(). */
	UPROPERTY(BlueprintReadWrite)
	class AWarframe* Player;

	/** Damage popup begin. */
	UPROPERTY(EditAnywhere)
	int32 DamageTextCapacity = 30;

	UPROPERTY()
	TArray<class UDamagePopup*> DamageTextPool;

	int32 DamageTextHead;
	int32 DamageTextTail;
	/** Damage popup end. */

	UPROPERTY(Meta = (BindWidget))
		UCanvasPanel* MainCanvasPanel;
	UPROPERTY(Meta = (BindWidget))
		class UMinimap* Minimap;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox* MissionPanel;
	UPROPERTY(Meta = (BindWidget))
		class UStatusDisplay* StatusDisplay;
	UPROPERTY(Meta = (BindWidget))
		UImage* HealthSplash;
	UPROPERTY(Meta = (BindWidget))
		UImage* ShieldSplash;

	/** Player info panel begin. */
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* PlayerShield;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* PlayerHealth;
	UPROPERTY(Meta = (BindWidget))
		UProgressBar* PlayerShieldPercent;
	UPROPERTY(Meta = (BindWidget))
		UProgressBar* PlayerHealthPercent;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* PlayerName;
	/** Player info panel end. */
	
	/** Companion info panel begin. */
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* CompanionShield;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* CompanionHealth;
	UPROPERTY(Meta = (BindWidget))
		UProgressBar* CompanionShieldPercent;
	UPROPERTY(Meta = (BindWidget))
		UProgressBar* CompanionHealthPercent;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* CompanionName;
	/** Companion info panel end. */

	/** Weapon info panel begin. */
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* WeaponMagazine;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* WeaponAmmo;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* WeaponName;
	/** Weapon info panel end. */

	/** Ability info panel begin. */
	UPROPERTY(Meta = (BindWidget))
		UProgressBar* EnergyPercent;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* Energy;
	/** Ability info panel end. */

	// Timer handle.
	FTimerHandle HealthSplashTimer;
	FTimerHandle ShieldSplashTimer;
};
