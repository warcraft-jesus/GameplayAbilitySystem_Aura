#include "Player/AuraPlayerController.h"
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
	Super::SetupInputComponent();

	InputComponent;
}
