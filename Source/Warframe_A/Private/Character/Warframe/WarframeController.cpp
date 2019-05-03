
#include "Character/Warframe/WarframeController.h"
#include "Character/Warframe/StateMachine/WarframeStateMachineComponent.h"
#include "Character/Warframe/WarframeTargetSelectionComponent.h"
#include "Character/Warframe/Warframe.h"

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/Engine/Public/TimerManager.h"


AWarframeController::AWarframeController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	TargetSelection = ObjectInitializer.CreateDefaultSubobject<UWarframeTargetSelectionComponent>(this, "TargetSelection");
}

void AWarframeController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AWarframeCharacter* Character = Cast<AWarframeCharacter>(InPawn);
	
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

void AWarframeController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Action handlers binding.
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AWarframeController::OnJumpPressed);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AWarframeController::OnCrouchPressed);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &AWarframeController::OnCrouchReleased);
	InputComponent->BindAction("ToggleCrouch", EInputEvent::IE_Pressed, this, &AWarframeController::OnToggleCrouchPressed);
	InputComponent->BindAction("PrimaryFire", EInputEvent::IE_Pressed, this, &AWarframeController::OnPrimaryFirePressed);
	InputComponent->BindAction("PrimaryFire", EInputEvent::IE_Released, this, &AWarframeController::OnPrimaryFireReleased);
	InputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &AWarframeController::OnAimPressed);
	InputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &AWarframeController::OnAimReleased);
	InputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AWarframeController::OnReloadPressed);
	InputComponent->BindAction("SwitchWeapon", EInputEvent::IE_Pressed, this, &AWarframeController::OnSwitchWeaponPressed);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AWarframeController::OnSprintPressed);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AWarframeController::OnSprintReleased);
	InputComponent->BindAction("AbilityA", EInputEvent::IE_Pressed, this, &AWarframeController::OnAbilityAPressed);
	InputComponent->BindAction("AbilityA", EInputEvent::IE_Released, this, &AWarframeController::OnAbilityAReleased);
	InputComponent->BindAction("AbilityB", EInputEvent::IE_Pressed, this, &AWarframeController::OnAbilityBPressed);
	InputComponent->BindAction("AbilityB", EInputEvent::IE_Released, this, &AWarframeController::OnAbilityBReleased);
	InputComponent->BindAction("AbilityC", EInputEvent::IE_Pressed, this, &AWarframeController::OnAbilityCPressed);
	InputComponent->BindAction("AbilityC", EInputEvent::IE_Released, this, &AWarframeController::OnAbilityCReleased);
	InputComponent->BindAction("AbilityD", EInputEvent::IE_Pressed, this, &AWarframeController::OnAbilityDPressed);
	InputComponent->BindAction("AbilityD", EInputEvent::IE_Released, this, &AWarframeController::OnAbilityDReleased);

	// Axis handlers binding.
	InputComponent->BindAxis("Turn", this, &AWarframeController::OnTurn);
	InputComponent->BindAxis("LookUp", this, &AWarframeController::OnLookUp);
	InputComponent->BindAxis("MoveForward", this, &AWarframeController::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &AWarframeController::OnMoveRight);
}

void AWarframeController::OnJumpPressed()
{
	Cast<AWarframeCharacter>(GetPawn())->GetStateMachine()->TriggerEvent(CastToUnderlyingType(EWarframeActionEvent::Jump));
}

void AWarframeController::OnCrouchPressed()
{
	Cast<UWarframeStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine())->bIsCrouching = true;
}

void AWarframeController::OnCrouchReleased()
{
	Cast<UWarframeStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine())->bIsCrouching = false;
}

void AWarframeController::OnToggleCrouchPressed()
{
	UWarframeStateMachineComponent* StateMachine = Cast<UWarframeStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine());

	StateMachine->bIsCrouching = !StateMachine->bIsCrouching;
}

void AWarframeController::OnPrimaryFirePressed()
{
	Cast<UWarframeStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine())->bIsFiring = true;
}

void AWarframeController::OnPrimaryFireReleased()
{
	Cast<UWarframeStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine())->bIsFiring = false;
}

void AWarframeController::OnAimPressed()
{
	Cast<UWarframeStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine())->IsAiming = true;
}

void AWarframeController::OnAimReleased()
{
	Cast<UWarframeStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine())->IsAiming = false;
}

void AWarframeController::OnReloadPressed()
{
	Cast<AWarframeCharacter>(GetPawn())->GetStateMachine()->TriggerEvent(CastToUnderlyingType(EWarframeActionEvent::Reload));
}

void AWarframeController::OnSwitchWeaponPressed()
{
	Cast<AWarframeCharacter>(GetPawn())->GetStateMachine()->TriggerEvent(CastToUnderlyingType(EWarframeActionEvent::SwitchWeapon));
}

void AWarframeController::OnSprintPressed()
{
	FTimerDelegate Delegate;
	Delegate.BindLambda([this](){
		Cast<UWarframeStateMachineComponent>(Cast<AWarframeCharacter>(this->GetPawn())->GetStateMachine())->bIsSprinting = true;
		this->RollTimerHandle.Invalidate();
	});

	this->GetGameInstance()->GetTimerManager().SetTimer(RollTimerHandle, Delegate, this->RollTimeThreshold, false);
}

void AWarframeController::OnSprintReleased()
{
	if (RollTimerHandle.IsValid())
	{
		this->GetGameInstance()->GetTimerManager().ClearTimer(RollTimerHandle);

		Cast<AWarframeCharacter>(GetPawn())->GetStateMachine()->TriggerEvent(CastToUnderlyingType(EWarframeActionEvent::Roll));
	}
	else
	{
		Cast<UWarframeStateMachineComponent>(Cast<AWarframeCharacter>(GetPawn())->GetStateMachine())->bIsSprinting = false;
	}
}

void AWarframeController::OnAbilityAPressed()
{
	Cast<AWarframe>(GetPawn())->CastAbility(0, 0.0f);
}

void AWarframeController::OnAbilityAReleased()
{}

void AWarframeController::OnAbilityBPressed()
{
	Cast<AWarframe>(GetPawn())->CastAbility(1, 0.0f);
}

void AWarframeController::OnAbilityBReleased()
{}

void AWarframeController::OnAbilityCPressed()
{
	Cast<AWarframe>(GetPawn())->CastAbility(2, 0.0f);
}

void AWarframeController::OnAbilityCReleased()
{}

void AWarframeController::OnAbilityDPressed()
{
	Cast<AWarframe>(GetPawn())->CastAbility(3, 0.0f);
}

void AWarframeController::OnAbilityDReleased()
{}

void AWarframeController::OnTurn(float Value)
{
	GetPawn()->AddControllerYawInput(Value);
}

void AWarframeController::OnLookUp(float Value)
{
	GetPawn()->AddControllerPitchInput(Value);
}

void AWarframeController::OnMoveForward(float Value)
{
	FRotator Rotator(0.0f, GetPawn()->GetControlRotation().Yaw, 0.0f);

	GetPawn()->AddMovementInput(Rotator.Vector(), Value);
}

void AWarframeController::OnMoveRight(float Value)
{
	FRotator Rotator(0.0f, GetPawn()->GetControlRotation().Yaw, 0.0f);

	FVector RightVector = FRotationMatrix(Rotator).GetScaledAxis(EAxis::Y);

	GetPawn()->AddMovementInput(RightVector, Value);
}
