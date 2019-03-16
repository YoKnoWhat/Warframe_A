// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CharacterWidgetComponent.h"
#include "Character/WarframeCharacter.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "UI/CharacterWidget.h"


UCharacterWidgetComponent::UCharacterWidgetComponent(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer)
{
	this->Space = EWidgetSpace::Screen;
}

void UCharacterWidgetComponent::InitWidget()
{
	// Override default widget class first.
	this->WidgetClass = FWarframeConfigSingleton::Instance().FindResourceClass("UI_CharacterHeaderPanel");

	Super::InitWidget();

	UCharacterWidget *CharacterWidget = Cast<UCharacterWidget>(this->Widget);

	if (CharacterWidget)
	{
		CharacterWidget->SetWarframeCharacter(Cast<AWarframeCharacter>(this->GetOwner()));
		CharacterWidget->Init();
	}
}

