// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractActor.h"
#include "Data/SaveGameStructs.h"
#include "TranslatableGlyph.generated.h"

/**
 * 
 */
UCLASS()
class COMMONUI_FPSBASE_API ATranslatableGlyph : public AInteractActor
{
	GENERATED_BODY()

public:

	ATranslatableGlyph();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isTranslated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Index;
	
	UFUNCTION(BlueprintCallable)
	void SetStateFromSaveData(const FWorldSave& WorldSaveData);
};
