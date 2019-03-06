// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/TennoCharacter.h"
#include "Character/TennoMovementComponent.h"
#include "UI/CharacterWidget.h"
#include "UI/CharacterWidgetComponent.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"


// Sets default values
// ATennoCharacter::ATennoCharacter()
// {
//  	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;
// 
// }

ATennoCharacter::ATennoCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UTennoMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATennoCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATennoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->TryFindTarget();
}

// Called to bind functionality to input
void ATennoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATennoCharacter::Init(ECharacterID CharacterID, uint32 Level)
{
	Super::Init(CharacterID, Level);
}

void ATennoCharacter::TryFindTarget()
{
	UCameraComponent *CameraComponent = Cast<UCameraComponent>(this->GetComponentByClass(UCameraComponent::StaticClass()));

	FCollisionQueryParams RV_TraceParams;
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.AddIgnoredActor(this);

	AWarframeCharacter* PrevTarget = Cast<AWarframeCharacter>(this->SelectedTarget.Actor.Get());

	if (GetWorld()->LineTraceSingleByChannel(
		this->SelectedTarget,
		CameraComponent->GetComponentLocation(),
		CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 10000.0f,
		ECC_Pawn,
		RV_TraceParams) == false)
	{
		this->SelectedTarget.ImpactPoint = this->SelectedTarget.TraceEnd;
	}

	AWarframeCharacter* NewTarget = Cast<AWarframeCharacter>(this->SelectedTarget.Actor.Get());

	if (PrevTarget != NewTarget)
	{
		if (PrevTarget != nullptr)
		{
			PrevTarget->OnUnselected();
		}
		if (NewTarget != nullptr)
		{
			NewTarget->OnSelected();
		}
	}
}
