// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Data/SaveGameStructs.h"
#include "JamSaveGameObject.generated.h"

/**
 * 
 */
UCLASS()
class COMMONUI_FPSBASE_API UJamSaveGameObject : public USaveGame
{
	GENERATED_BODY()
	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Saves")
	FAudioSave AudioData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Saves")
	FControlsSave SavedControlsData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Saves")
	FPlayerSave PlayerData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Saves")
	FGraphicsSettingsSave GraphicsData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Saves")
	FWorldSave WorldData;
};
