// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/SaveGameStructs.h"
#include "Game/Save/SaveGameInterface.h"
#include "JamGameInstanceSubsystem.generated.h"

class USaveGame;
class UJamSaveGameObject;
class UGameUserSettings;

/**
 * 
 */
UCLASS()
class COMMONUI_FPSBASE_API UJamGameInstanceSubsystem : public UGameInstanceSubsystem, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	UJamGameInstanceSubsystem();

public:
	// The object that stores the actual data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	UJamSaveGameObject* GameSaveObject;
	
	// The save name of the slot we want to save to
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	FString SaveSlotName;

	// A bool that checks whether or not we wiped our player save data (like if the player clicked "New Game" instead of "Continue" - set in ClearPlayerSaveData and dictates various button states in the UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	bool PlayerDataCleared;

	// Allows us to replace any saved Player Data with blank data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FPlayerSave EmptyPlayerDataStruct;

	// Used to dictate the state of various buttons in the UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	bool DoesSaveExist;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	TSubclassOf<USaveGame> SaveObjectClass;

	FGraphicsSettingsSave GraphicsSettings;
	UGameUserSettings* GameUserSettings;

	

public:
	// override the subsystems Initialize function so I can add functionality to it.
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Used when the player clicks "New Game" - this will wipe Player Data, but not controls, audio, or graphics settings. Basically just restarts gameplay from 0
	UFUNCTION(BlueprintCallable)
	void ClearPlayerSaveData();

	UFUNCTION()
	void SetGraphicsSettingsFromSaveData();

	void SetResolution();

	// Because we implement the ISaveGameInterface, we MUST override these functions
	virtual UJamSaveGameObject* GetSaveGameData_Implementation() const override;
	virtual void LoadGameData_Implementation() override;
	virtual void SaveGameData_Implementation() override;
	virtual void SetPlayerSaveData_Implementation(const FPlayerSave& PlayerData) override;
	virtual void SetControlsSaveData_Implementation(const FControlsSave& SavedControlsData) override;
	virtual void SetGraphicsSaveData_Implementation(const FGraphicsSettingsSave& GraphicsData) override;
	virtual void SetAudioSettingsSaveData_Implementation(const FAudioSave& AudioData) override;
	virtual void SetWorldSaveData_Implementation(const FWorldSave& WorldData) override;
	// End interface
};
