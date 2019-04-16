
#include "Character/Warframe/WarframeController.h"
#include "Character/Warframe/WarframeTargetSelectionComponent.h"
#include "Character/WarframeCharacter.h"

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"


AWarframeController::AWarframeController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	TargetSelection = ObjectInitializer.CreateDefaultSubobject<UWarframeTargetSelectionComponent>(this, "TargetSelection");
}

void AWarframeController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AWarframeCharacter* Character = Cast<AWarframeCharacter>(InPawn);

	Character->SetGenericTeamId(FGenericTeamId(CastToUnderlyingType(EWarframeTeamID::Player)));
	
	/** Dynamic spring arm component creation. */
	USpringArmComponent* SpringArmComponent = NewObject<USpringArmComponent>(InPawn);
	SpringArmComponent->SetupAttachment(Character->GetCapsuleComponent());
	SpringArmComponent->RegisterComponent();
	SpringArmComponent->TargetArmLength = 200.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	/** Dynamic camera component creation. */
	UCameraComponent* CameraComponent = NewObject<UCameraComponent>(InPawn);
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->RegisterComponent();
	CameraComponent->RelativeLocation = FVector(0.000000, 50.000000, 50.000000);

	FViewTargetTransitionParams TransitionParams;
	this->SetViewTarget(InPawn, TransitionParams);
}

void AWarframeController::UnPossess()
{
	if (GetPawn() != nullptr)
	{
		USpringArmComponent* SpringArmComponent = Cast<USpringArmComponent>(GetPawn()->GetComponentByClass(USpringArmComponent::StaticClass()));
		if (SpringArmComponent != nullptr)
		{
			SpringArmComponent->DestroyComponent();
		}

		// Camera comopnent is a child of spring arm component, so there is no need to destroy it explicitly,
	}

	Super::UnPossess();
}

void AWarframeController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (GetPawn() != nullptr)
	{
		TargetSelection->UpdateSelectedTarget();
	}
}
