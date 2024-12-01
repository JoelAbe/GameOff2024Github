// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/AIData.h"
#include "CombatManager.generated.h"

class ANPCBaseCharacter;

UCLASS()
class COMMONUI_FPSBASE_API ACombatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Public Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int LightTokensAvailable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int MaxLightTokens;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	EClassType LightToken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int MediumTokensAvailable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int MaxMediumTokens;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	EClassType MediumToken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int HeavyTokensAvailable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int MaxHeavyTokens;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	EClassType HeavyToken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TokenCooldownDuration;

	// Used as a queue to take tokens off cooldown that got returned first
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<EClassType> TokensOnCooldownArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TMap<EClassType, int32> TokensOnCooldown;

	FTimerHandle TokenCooldownTimerHandle;

/////////////////////////////////////////////////////////////////////

	// Public Functions

	void InitializeTokenValues();

	UFUNCTION(BlueprintCallable)
	int GetAvailableTokensByType(EClassType ClassType);

	UFUNCTION(BlueprintCallable)
	void RemoveTokenByType(EClassType ClassType);

	UFUNCTION(BlueprintCallable)
	void TakeTokenOffCooldown();

	UFUNCTION(BlueprintCallable)
	void PutTokenOnCooldown(EClassType ClassType);

	UFUNCTION(BlueprintCallable)
	void ManageTokenRequest(ANPCBaseCharacter* NPC);
};
