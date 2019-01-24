// Fill out your copyright notice in the Description page of Project Settings.

#include "WarframeConfigSingleton.h"
#include "CharacterWidget.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Core/Public/Misc/ConfigCacheIni.h"


UWarframeConfigSingleton::UWarframeConfigSingleton(const FObjectInitializer& ObjectInitializer)
{
	CharacterWidgetClass = UCharacterWidget::StaticClass();
}

UWarframeConfigSingleton* UWarframeConfigSingleton::Instance()
{
	FString WarframeConfigSingleton_BPRef;

	GConfig->GetString(TEXT("ConfigurableStrings"), TEXT("WarframeConfigSingleton_BPRef"), WarframeConfigSingleton_BPRef, GGameIni);

	static ConstructorHelpers::FObjectFinder<UClass> GlobalConfigClass(*WarframeConfigSingleton_BPRef);
	
	if (GlobalConfigClass.Succeeded())
	{
		return Cast<UWarframeConfigSingleton>(GlobalConfigClass.Object->GetDefaultObject());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find specified override of WarframeConfigSingleton."));

		return Cast<UWarframeConfigSingleton>(UWarframeConfigSingleton::StaticClass()->GetDefaultObject());
	}

}
