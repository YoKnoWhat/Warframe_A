// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterWidget.h"
#include "Runtime/Engine/Public/TimerManager.h"


TArray<UCharacterWidget*> UCharacterWidget::VisibleWidgetArray;

UCharacterWidget::UCharacterWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	this->Visibility = ESlateVisibility::Hidden;
}

void UCharacterWidget::BeginDestroy()
{
	Super::BeginDestroy();

	if (FadeOutTimer.IsValid())
	{
		// Game instance might be nullptr when shutting down.
		UGameInstance* GameInstance = this->GetGameInstance();
		if (GameInstance != nullptr)
		{
			GameInstance->GetTimerManager().ClearTimer(FadeOutTimer);
		}
	}
	VisibleWidgetArray.Remove(this);
}

void UCharacterWidget::SetWarframeCharacter(AWarframeCharacter *WarframeCharacter_)
{
	WarframeCharacter = WarframeCharacter_;
}

void UCharacterWidget::OnSelected()
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

void UCharacterWidget::OnUnselected()
{
	CurrentOpacity = 1.0f;
	OpacityStep = 1.0f / 60.0f;

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UCharacterWidget::FadeOut);

	this->GetGameInstance()->GetTimerManager().SetTimer(FadeOutTimer, Delegate, 1.0f / 60.0f, true);
}

void UCharacterWidget::FadeOut()
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
