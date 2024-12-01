// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JamPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Player/JamPlayerCharacter.h"
#include "Game/Save/SaveGameInterface.h"
#include "Game/Save/JamSaveGameObject.h"
#include "Game/JamGameInstanceSubsystem.h"

AJamPlayerController::AJamPlayerController()
{
	BaseTurnRate = 50.0f;
	InvertY = false;
}

void AJamPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializeGameInstanceSubsystem();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
		//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, TEXT("Mapping context added successfully"));
	}

	// We want to set the controls of the player controller to whatever the player last set them to, based on their save data.
	// So we get that data from the game instance and set the player controller's variables with that data.
	SavedControlsData = GetSaveDataFromGameInstance();
	SetControlsFromSaveData(SavedControlsData);

	FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (LevelName == FString("MainMenuMap")) return;

	PlayerCharacter = Cast<AJamPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	SetupInputComponent(InputComponent);
}

void AJamPlayerController::SetupInputComponent(UInputComponent* InInputComponent)
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (LevelName == FString("MainMenuMap")) return;
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, PlayerCharacter, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, PlayerCharacter, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJamPlayerController::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJamPlayerController::Look);

		// GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Blue, TEXT("SetupInputComponent() ran successfully"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("SetupInputComponent() failed!"));
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AJamPlayerController::SetControlsFromSaveData(const FControlsSave& ControlsSaveData)
{
	// This check just makes sure that the save data is valid, because the BaseTurnRate value in the struct is 0
	// and players would never set it to 0 manually, because they wouldn't be able to move.
	// If the data isn't valid, return early
	if (ControlsSaveData.BaseTurnRate == 0) return;

	InvertY = ControlsSaveData.InvertY;
	BaseTurnRate = ControlsSaveData.BaseTurnRate;
}

FControlsSave& AJamPlayerController::GetSaveDataFromGameInstance()
{
	// Get the game instance, and call its interface function GetSaveGameData so we can return the ControlsData struct
	// Store that retrieved data in SaveGameData
	UJamSaveGameObject* SaveGameData = ISaveGameInterface::Execute_GetSaveGameData(GameInstanceSubsystem);

	if (SaveGameData)
	{
		return SaveGameData->SavedControlsData;
	}
	else
	{
		static FControlsSave EmptyControlsData;
		return EmptyControlsData;
	}
}

FControlsSave& AJamPlayerController::GetControllerSaveData()
{
	// Update the SavedControlsData variable with current data about the controls, then return the struct.
	SavedControlsData.BaseTurnRate = BaseTurnRate;
	SavedControlsData.InvertY = InvertY;

	return SavedControlsData;
}

void AJamPlayerController::InitializeGameInstanceSubsystem()
{
	if (!GameInstanceSubsystem)
	{
		GameInstanceSubsystem = GetGameInstance()->GetSubsystem<UJamGameInstanceSubsystem>();
		if (!GameInstanceSubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameInstanceSubsystem could not be initialized in AJamPlayerController."));
		}
	}
}

void AJamPlayerController::Move(const FInputActionValue& Value)
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (LevelName == FString("MainMenuMap")) return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	
	PlayerPawn->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
	PlayerPawn->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);
}

void AJamPlayerController::Look(const FInputActionValue& Value)
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (LevelName == FString("MainMenuMap")) return;

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	// add yaw and pitch input to controller
	PlayerCharacter->AddControllerYawInput(LookAxisVector.X * BaseTurnRate * DeltaTime);

	float NegativeOne = -1.0f;
	float InvertedPitchInput = LookAxisVector.Y * BaseTurnRate * DeltaTime * NegativeOne;
	float PitchInput = LookAxisVector.Y * BaseTurnRate * DeltaTime;

	// if InvertY = true, set Result to InvertedPitchInput
	float Result = InvertY ? InvertedPitchInput : PitchInput; 
	PlayerCharacter->AddControllerPitchInput(Result);
}
