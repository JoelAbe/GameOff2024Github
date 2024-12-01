// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/JamGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Game/Save/JamSaveGameObject.h"

UJamGameInstanceSubsystem::UJamGameInstanceSubsystem()
{
	GameSaveObject = nullptr;
	SaveSlotName = TEXT("SaveSlot");
	SaveObjectClass = UJamSaveGameObject::StaticClass();
	DoesSaveExist = false;
	PlayerDataCleared = false;
}

void UJamGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadGameData_Implementation();
	SetGraphicsSettingsFromSaveData();
}

void UJamGameInstanceSubsystem::ClearPlayerSaveData()
{
}

void UJamGameInstanceSubsystem::SetGraphicsSettingsFromSaveData()
{
	if (GameSaveObject)
	{
		SetResolution();
		GameUserSettings->SetOverallScalabilityLevel(GraphicsSettings.QualityIndex);
		GameUserSettings->SetViewDistanceQuality(GraphicsSettings.ViewDistanceIndex);
		GameUserSettings->SetAntiAliasingQuality(GraphicsSettings.AntiAliasingIndex);
		GameUserSettings->SetPostProcessingQuality(GraphicsSettings.PostProcessingIndex);
		GameUserSettings->SetShadowQuality(GraphicsSettings.ShadowsIndex);
		GameUserSettings->SetTextureQuality(GraphicsSettings.TexturesIndex);
		GameUserSettings->SetVisualEffectQuality(GraphicsSettings.EffectsIndex);
		GameUserSettings->SetShadingQuality(GraphicsSettings.ShadingIndex);
		GameUserSettings->ApplySettings(false);
	}
}

void UJamGameInstanceSubsystem::SetResolution()
{
	switch (GraphicsSettings.ResolutionIndex)
	{
	case 0:
		GameUserSettings->SetScreenResolution(FIntPoint(1280, 720));
		break;
	case 1:
		GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
		break;
	case 2:
		GameUserSettings->SetScreenResolution(FIntPoint(2560, 1440));
		break;
	case 3:
		GameUserSettings->SetScreenResolution(FIntPoint(3840, 2160));
	}
}


UJamSaveGameObject* UJamGameInstanceSubsystem::GetSaveGameData_Implementation() const
{
	return GameSaveObject;
}

void UJamGameInstanceSubsystem::LoadGameData_Implementation()
{
	// Check if the save game exists
	DoesSaveExist = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);

	// If it does, load it and cast it to our save game object so we have a reference to it
	if (DoesSaveExist)
	{
		USaveGame* LoadedGameObject = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
		GameSaveObject = Cast<UJamSaveGameObject>(LoadedGameObject);
	}
	// Otherwise, create a new save game object
	else
	{
		USaveGame* NewGameObject = UGameplayStatics::CreateSaveGameObject(SaveObjectClass);
		GameSaveObject = Cast<UJamSaveGameObject>(NewGameObject);
	}

	// Initialize Member variables
	GraphicsSettings = GameSaveObject->GraphicsData;
	GameUserSettings = UGameUserSettings::GetGameUserSettings();
}

void UJamGameInstanceSubsystem::SaveGameData_Implementation()
{
	FAsyncSaveGameToSlotDelegate OnGameSavedDelegate;
	UGameplayStatics::AsyncSaveGameToSlot(GameSaveObject, SaveSlotName, 0, OnGameSavedDelegate);
}

void UJamGameInstanceSubsystem::SetPlayerSaveData_Implementation(const FPlayerSave& PlayerData)
{
	GameSaveObject->PlayerData = PlayerData;
	PlayerDataCleared = false;
}

void UJamGameInstanceSubsystem::SetControlsSaveData_Implementation(const FControlsSave& SavedControlsData)
{
	GameSaveObject->SavedControlsData = SavedControlsData;
}

void UJamGameInstanceSubsystem::SetGraphicsSaveData_Implementation(const FGraphicsSettingsSave& GraphicsData)
{
	GameSaveObject->GraphicsData = GraphicsData;
}

void UJamGameInstanceSubsystem::SetAudioSettingsSaveData_Implementation(const FAudioSave& AudioData)
{
	GameSaveObject->AudioData = AudioData;
}

void UJamGameInstanceSubsystem::SetWorldSaveData_Implementation(const FWorldSave& WorldData)
{
	GameSaveObject->WorldData = WorldData;
}
