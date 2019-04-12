// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Warframe/Warframe.h"
#include "Character/Warframe/AbilityObject.h"
#include "Character/Warframe/WarframeMovementComponent.h"
#include "Character/Warframe/StateMachine/WarframeLowerStates.h"
#include "Character/Warframe/StateMachine/WarframeUpperStates.h"
#include "Character/Warframe/StateMachine/WarframeAimStates.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Gameplay/WarframeGameInstance.h"
#include "Gameplay/WarframeConfigSingleton.h"
#include "UI/CharacterWidget.h"
#include "UI/CharacterWidgetComponent.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"


AWarframe::AWarframe(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UWarframeMovementComponent>(ACharacter::CharacterMovementComponentName)
			.SetDefaultSubobjectClass<UWarframeStateMachineComponent>("StateMachine"))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Abilities[0] = new FAbilityObject_Null();
	Abilities[1] = new FAbilityObject_Null();
	Abilities[2] = new FAbilityObject_Null();
	Abilities[3] = new FAbilityObject_Null();

	StateMachineComponent->ClearAllLayers();
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeLowerState_AimGliding);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeLowerState_BulletJumping);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeLowerState_Crouching);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeLowerState_DoubleJumping);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeLowerState_Falling);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeLowerState_Idle);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeLowerState_Jumping);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeLowerState_Rolling);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeLowerState_Sliding);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Lower), new FWarframeLowerState_Sprinting);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Upper), new FWarframeUpperState_Firing);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Upper), new FWarframeUpperState_Idle);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Upper), new FWarframeUpperState_Ironsight);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Upper), new FWarframeUpperState_Reloading);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Upper), new FWarframeUpperState_WeaponSwitching);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Aim), new FWarframeAimState_Aiming);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EWarframeStateLayer::Aim), new FWarframeAimState_Idle);
}

AWarframe::~AWarframe()
{
	for (uint32 i = 0; i < 4; ++i)
	{
		delete Abilities[i];
	}
}

// Called when the game starts or when spawned
void AWarframe::BeginPlay()
{
	Super::BeginPlay();

	TMap<int32, int32> InitStateIDs = {
		{ CastToUnderlyingType(EWarframeStateLayer::Lower), CastToUnderlyingType(EWarframeLowerState::Idle) },
		{ CastToUnderlyingType(EWarframeStateLayer::Upper), CastToUnderlyingType(EWarframeUpperState::Idle) },
		{ CastToUnderlyingType(EWarframeStateLayer::Aim), CastToUnderlyingType(EWarframeAimState::Idle) }
	};

	this->StateMachineComponent->Init(InitStateIDs);
}

void AWarframe::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AWarframe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Regenerate energy anyway for now.
	CurrentEnergy += 0.016667f;
	if (CurrentEnergy > MaxEnergy)
	{
		CurrentEnergy = MaxEnergy;
	}

	for (FAbilityObject* Ability : Abilities)
	{
		Ability->Tick(DeltaTime);
	}
}

void AWarframe::Init(ECharacterID InCharacterID)
{
	Super::Init(InCharacterID);

	AbilityStrength = 1.0f;
	AbilityRange = 1.0f;
	AbilityEfficiency = 1.0f;
	AbilityDuration = 1.0f;

	// Apply mods.
}

void AWarframe::SetLevel(uint32 InLevel)
{
	Super::SetLevel(InLevel);

	UWarframeGameInstance *GameInstance = Cast<UWarframeGameInstance>(this->GetGameInstance());

	const FWarframeInfo* WarframeInfo = GameInstance->GetWarframeInfo(CharacterID);

	if (WarframeInfo != nullptr)
	{
		this->MaxEnergy = this->CurrentEnergy = WarframeInfo->Energy + (Level - 1) * 10;
	}

	// Ability 0.
	if (Level >= 18)
	{
		Abilities[0]->OnLevelChanged(3);
	}
	else if (Level >= 14)
	{
		Abilities[0]->OnLevelChanged(2);
	}
	else if (Level >= 7)
	{
		Abilities[0]->OnLevelChanged(1);
	}
	else
	{
		Abilities[0]->OnLevelChanged(0);
	}

	// Ability 1.
	if (Level >= 27)
	{
		Abilities[1]->OnLevelChanged(3);
	}
	else if (Level >= 22)
	{
		Abilities[1]->OnLevelChanged(2);
	}
	else if (Level >= 12)
	{
		Abilities[1]->OnLevelChanged(1);
	}
	else if (Level >= 3)
	{
		Abilities[1]->OnLevelChanged(0);
	}
	else
	{
		Abilities[1]->OnLevelChanged(-1);
	}

	// Ability 2.
	if (Level >= 28)
	{
		Abilities[2]->OnLevelChanged(3);
	}
	else if (Level >= 24)
	{
		Abilities[2]->OnLevelChanged(2);
	}
	else if (Level >= 16)
	{
		Abilities[2]->OnLevelChanged(1);
	}
	else if (Level >= 5)
	{
		Abilities[2]->OnLevelChanged(0);
	}
	else
	{
		Abilities[2]->OnLevelChanged(-1);
	}

	// Ability 3.
	if (Level >= 30)
	{
		Abilities[3]->OnLevelChanged(3);
	}
	else if (Level >= 25)
	{
		Abilities[3]->OnLevelChanged(2);
	}
	else if (Level >= 20)
	{
		Abilities[3]->OnLevelChanged(1);
	}
	else if (Level >= 10)
	{
		Abilities[3]->OnLevelChanged(0);
	}
	else
	{
		Abilities[3]->OnLevelChanged(-1);
	}
}

void AWarframe::GainEnergy(float Value)
{
	CurrentEnergy += Value;

	if (CurrentEnergy > MaxEnergy)
	{
		CurrentEnergy = MaxEnergy;
	}
}

void AWarframe::DrainEnergy(float Value)
{
	if (Value > CurrentEnergy)
	{
		CurrentEnergy = 0;
	}
	else
	{
		CurrentEnergy -= Value;
	}
}

bool AWarframe::CastAbilityBP(int32 AbilityIndex, float Charge)
{
	return this->CastAbility(AbilityIndex, Charge);
}

bool AWarframe::CastAbility(uint32 AbilityIndex, float Charge)
{
	this->Abilities[AbilityIndex]->Cast(Charge);

	return true;
}
