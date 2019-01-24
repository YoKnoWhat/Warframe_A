// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterWidgetComponent.h"
#include "CharacterWidget.h"
#include "WarframeConfigSingleton.h"


UCharacterWidgetComponent::UCharacterWidgetComponent(const FObjectInitializer &ObjectInitializer) :
	Super(ObjectInitializer)
{
	this->Space = EWidgetSpace::Screen;
	this->WidgetClass = UWarframeConfigSingleton::Instance()->CharacterWidgetClass;
}

void UCharacterWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UCharacterWidget *CharacterWidget = Cast<UCharacterWidget>(this->Widget);

	if (CharacterWidget)
	{
		CharacterWidget->SetWarframeCharacter(Cast<AWarframeCharacter>(this->GetOwner()));
	}
}

