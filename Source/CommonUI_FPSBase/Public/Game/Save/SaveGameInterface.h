// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/SaveGameStructs.h"
#include "SaveGameInterface.generated.h"

class USaveGame;
class UJamSaveGameObject;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONUI_FPSBASE_API ISaveGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// I want these functions to also have blueprint support, so I need to create 2 versions. To call these functions in blueprint
	// I'll need to simply call GetSaveGameData(), or LoadGameData() - I won't have to worry about the _Implementation versions.
	// But if I want to use these functions in C++, I'll have to override the _Implementation versions.
	// Using "= 0" at the end of the _Implementation versions REQUIRES any classes that implment this interface to also implement those functions.
	// If they don't, the project won't build. If I want to override those functions in JamPlayerCharacter (which I'll definitely do), it would look like this:
	// virtual void LoadGameData_Implementation() override;
	// Then i can simply write whatever code I want in the .cpp file of that class

	// Function used to return save data from the class that implements this interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	UJamSaveGameObject* GetSaveGameData() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void LoadGameData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SaveGameData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetPlayerSaveData(const FPlayerSave& PlayerData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetControlsSaveData(const FControlsSave& SavedControlsData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetGraphicsSaveData(const FGraphicsSettingsSave& GraphicsData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetAudioSettingsSaveData(const FAudioSave& AudioData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetWorldSaveData(const FWorldSave& WorldData);

	virtual UJamSaveGameObject* GetSaveGameData_Implementation() const = 0;
	virtual void LoadGameData_Implementation() = 0;
	virtual void SaveGameData_Implementation() = 0;
	virtual void SetPlayerSaveData_Implementation(const FPlayerSave& PlayerData) = 0;
	virtual void SetControlsSaveData_Implementation(const FControlsSave& SavedControlsData) = 0;
	virtual void SetGraphicsSaveData_Implementation(const FGraphicsSettingsSave& GraphicsData) = 0;
	virtual void SetAudioSettingsSaveData_Implementation(const FAudioSave& AudioData) = 0;
	virtual void SetWorldSaveData_Implementation(const FWorldSave& WorldData) = 0;
};
