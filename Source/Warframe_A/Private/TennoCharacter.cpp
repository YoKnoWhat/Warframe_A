// Fill out your copyright notice in the Description page of Project Settings.

#include "TennoCharacter.h"
#include "TennoMovementComponent.h"


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

}

// Called to bind functionality to input
void ATennoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

