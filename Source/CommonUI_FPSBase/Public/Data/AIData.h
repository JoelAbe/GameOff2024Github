// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AIData.generated.h"

UENUM(BlueprintType)
enum class EClassType : uint8
{
	Light	UMETA(DisplayName = "Light"),
	Medium  UMETA(DisplayName = "Medium"),
	Heavy	UMETA(DisplayName = "Heavy")
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Moving			UMETA(DisplayName = "Moving"),
	Attacking		UMETA(DisplayName = "Attacking"),
	ReadyToAttack	UMETA(DisplayName = "ReadyToAttack"),
	Waiting			UMETA(DisplayName = "Waiting"),
	Taunting		UMETA(DisplayName = "Taunting"),
	Dead			UMETA(DisplayName = "Dead"),
	Stunned			UMETA(DisplayName = "Stunned")
};

/**
 * 
 */
UCLASS()
class COMMONUI_FPSBASE_API UAIData : public UObject
{
	GENERATED_BODY()
	
};
