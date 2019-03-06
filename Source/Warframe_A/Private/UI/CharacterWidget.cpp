// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CharacterWidget.h"


UCharacterWidget::UCharacterWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{}

void UCharacterWidget::SetWarframeCharacter(AWarframeCharacter *WarframeCharacter_)
{
	WarframeCharacter = WarframeCharacter_;
}

void UCharacterWidget::Init()
{}

void UCharacterWidget::OnSelected()
{}

void UCharacterWidget::OnUnselected()
{}
