// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/VerticalBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "StatusDisplay.generated.h"


UCLASS()
class WARFRAME_A_API UStatusDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UStatusDisplay(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void RefreshStatuses(class AWarframeCharacter* Character);

protected:
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	SlashPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		SlashText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	ImpactPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		ImpactText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	PuncturePanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		PunctureText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	HeatPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		HeatText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	ColdPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		ColdText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	ElectricityPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		ElectricityText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	ToxinPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		ToxinText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	BlastPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		BlastText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	RadiationPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		RadiationText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	GasPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		GasText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	MagneticPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		MagneticText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	ViralPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		ViralText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	CorrosivePanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		CorrosiveText;
	UPROPERTY(Meta = (BindWidget))
		UVerticalBox*	VoidPanel;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock*		VoidText;
};
