
#include "Character/Grineer/Grineer.h"
#include "Character/Grineer/StateMachine/GrineerStateMachineComponent.h"
#include "Character/Grineer/StateMachine/GrineerLowerStates.h"
#include "Character/Grineer/StateMachine/GrineerUpperStates.h"


AGrineer::AGrineer(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGrineerStateMachineComponent>("StateMachine"))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateMachineComponent->ClearAllLayers();
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EGrineerStateLayer::Lower), new FGrineerLowerState_Crouching);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EGrineerStateLayer::Lower), new FGrineerLowerState_Falling);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EGrineerStateLayer::Lower), new FGrineerLowerState_Idle);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EGrineerStateLayer::Lower), new FGrineerLowerState_Jumping);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EGrineerStateLayer::Lower), new FGrineerLowerState_Sprinting);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EGrineerStateLayer::Upper), new FGrineerUpperState_Firing);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EGrineerStateLayer::Upper), new FGrineerUpperState_Idle);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EGrineerStateLayer::Upper), new FGrineerUpperState_Ironsight);
	StateMachineComponent->AddStateObject(CastToUnderlyingType(EGrineerStateLayer::Upper), new FGrineerUpperState_Reloading);
}

void AGrineer::BeginPlay()
{
	Super::BeginPlay();
}

void AGrineer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGrineer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrineer::Init(ECharacterID InCharacterID)
{
	Super::Init(InCharacterID);
}
