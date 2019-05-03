
#pragma once

#include "Character/StateMachine/WarframeCharacterAIStateMachineComponent.h"
#include "Character/Corpus/StateMachine/CorpusLowerStates.h"
#include "Character/Corpus/StateMachine/CorpusUpperStates.h"
#include "CorpusStateMachineComponent.generated.h"


UENUM(BlueprintType)
enum class ECorpusStateLayer : uint8
{
	Lower,
	Upper,
};

UENUM(BlueprintType)
enum class ECorpusLowerState : uint8
{
	Crouching,
	Dead,
	Falling,
	Idle,
	Jumping,
	Sprinting,
	AtCoverStanding,
	AtCoverCrouching,
	AtCoverFiringStanding,
	AtCoverFiringCrouching,
};

UENUM(BlueprintType)
enum class ECorpusUpperState : uint8
{
	Idle,
	Reloading,
	Firing,
	WeaponSwitching,
	Ironsight,
};

UENUM(BlueprintType)
enum class ECorpusActionEvent : uint8
{
	Jump,
	Reload,
	SwitchWeapon,
};

UCLASS()
class WARFRAME_A_API UCorpusStateMachineComponent : public UWarframeCharacterAIStateMachineComponent
{
	GENERATED_BODY()

public:
	UCorpusStateMachineComponent();

	virtual void Init(AWarframeCharacter* InCharacter, FStateMachineLayerInitializer& LayerInitializer)override;

	virtual void ReInit()override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)override;

};
