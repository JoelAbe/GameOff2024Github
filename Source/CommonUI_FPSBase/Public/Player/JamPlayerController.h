// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/SaveGameStructs.h"
#include "JamPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UJamGameInstanceSubsystem;
class AJamPlayerCharacter;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class COMMONUI_FPSBASE_API AJamPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

public:
	// Constructor
	AJamPlayerController();

	// The action used to look around
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// A reference to the player character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "References", meta = (AllowPrivateAccess = "true"))
	class AJamPlayerCharacter* PlayerCharacter;

	// Mouse sensitivity - How fast does the camera move with mouse movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	// Used to determine the up and down movement of the camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	bool InvertY;
	
	// Stores data retrieved from the Save Object that lives in the game instance. The struct gets reset with updated values any time we try to save the controls. This is basically a middle-man to pass save data around
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Saving")
	FControlsSave SavedControlsData;

	UJamGameInstanceSubsystem* GameInstanceSubsystem;

protected:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	void SetupInputComponent(UInputComponent* InputComponent);
	// End of APawn interface

public:
	// Sets player controller's current controls equal to the controls found in the save data
	UFUNCTION(BlueprintCallable)
	void SetControlsFromSaveData(const FControlsSave& ControlsSaveData);

	// Returns the player controller's settings found in the save data
	UFUNCTION(BlueprintCallable)
	FControlsSave& GetSaveDataFromGameInstance();

	// Returns the player controller's current settings
	UFUNCTION(BlueprintCallable)
	FControlsSave& GetControllerSaveData();

private:

	void InitializeGameInstanceSubsystem();
};
