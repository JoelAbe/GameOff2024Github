// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Data/SpellInventoryStructs.h"
#include "JamPlayerState.generated.h"

class UAttributesComponent;

/**
 * 
 */
UCLASS()
class COMMONUI_FPSBASE_API AJamPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	// Constructor
	AJamPlayerState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributesComponent* CharacterAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpell EquippedSpell;

	FORCEINLINE FSpell GetEquippedSpell() const { return EquippedSpell; }
	FORCEINLINE UAttributesComponent* GetCharacterAttributes() const { return CharacterAttributes; }
};
