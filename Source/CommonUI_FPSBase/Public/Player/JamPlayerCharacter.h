// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/JamCharacterBase.h"
#include "Data/SaveGameStructs.h"
#include "JamPlayerCharacter.generated.h"

class UCameraComponent;
class AJamPlayerController;
class AJamPlayerState;
class UAttributesComponent;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class COMMONUI_FPSBASE_API AJamPlayerCharacter : public AJamCharacterBase
{
	GENERATED_BODY()
	

public:
	// Constructor
	AJamPlayerCharacter();

	// The main camera for the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	// Defines how far the line trace will go from the camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractDistance;

	// Defines the actor the InteractTrace hits
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	AActor* LookAtActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	AJamPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	AJamPlayerState* PlayerStateRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	UAttributesComponent* Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float ManaChargeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float HealthChargeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float ManaChargeAmountPerTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float HealthChargeAmountPerTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	int32 DashCharges;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	int32 MaxDashCharges;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldownDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashInterpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dash")
	FVector DashTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dash")
	bool isDashing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	FTimerHandle FirstDashCooldownTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dash")
	bool hasRedPotion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dash")
	bool hasBlackPotion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dash")
	bool hasWhitePotion;


protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	// Function run on tick that will allow actors to be highlighted if we look at them. Simply draws a line out of the camera to a point in front of the player based on where the camera is looking.
	void InteractTrace();
	
	// Custom collision channel for enemies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TEnumAsByte<ECollisionChannel> EnemyChannel;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public:
	UFUNCTION(BlueprintCallable)
	FPlayerSave GetCharacterSaveData();

	UFUNCTION(BlueprintCallable)
	void SetPlayerStateFromSaveData(const FPlayerSave& PlayerSaveData);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageReceived();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerDataLoaded();

	UFUNCTION(BlueprintCallable)
	void StartDash();

	UFUNCTION(BlueprintCallable)
	void PerformDash();

	UFUNCTION(BlueprintCallable)
	void ReplenishDashCharge();

	UFUNCTION(BlueprintCallable)
	void HandleDashInterpolation(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void HandleDashCooldown();
};
