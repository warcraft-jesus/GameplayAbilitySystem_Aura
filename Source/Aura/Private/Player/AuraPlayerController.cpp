#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	// enable network replication
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	// call parent class BeginPlay
	Super::BeginPlay();
	// validate the input mapping context
	check(AuraContext);

	// get the Enhanced Input subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// ensure the subsystem is valid
	check(Subsystem);
	// add the input mapping context with priority 0
	Subsystem->AddMappingContext(AuraContext, 0);

	// enable mouse cursor visibility
	bShowMouseCursor = true;
	// set the default cursor type
	DefaultMouseCursor = EMouseCursor::Default;

	// configure the input mode for game and UI interaction
	FInputModeGameAndUI InputModeData;
	// allow free mouse movement (no viewport locking)
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// keep the cursor visible during input capture
	InputModeData.SetHideCursorDuringCapture(false);
	// apply the configured input mode
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	// call parent SetupInputComponent to ensure inherited functionality
	Super::SetupInputComponent();
	// cast the InputComponent to the EnhancedInputComponent type
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	// bind the Move action to the Move method when the action starts
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// get the 2D input axis vector from the input action value
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	// get the controller's rotation
	const FRotator Rotation = GetControlRotation();
	// create a rotation with only the yaw value (no pitch or roll)
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f);
	// calculate the forward direction based on the yaw rotation
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// calculate the right direction based on the yaw rotation
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// check if the player is controlling a pawn
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// apply movement input in the forward direction based on the Y axis input
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		// apply movement input in the right direction based on the X axis input
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

