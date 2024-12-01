// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMMONUI_FPSBASE_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	float MaxMana;

//////////////////////////////////////////////////////////////////

	// Functions
	UFUNCTION(BlueprintCallable)
	void InitializeAttributeValues();

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();
		
	UFUNCTION(BlueprintCallable)
	float GetCurrentMana();

	UFUNCTION(BlueprintCallable)
	float GetCurrentStamina();

	UFUNCTION(BlueprintCallable)
	void AddHealth(float HealthToAdd);

	UFUNCTION(BlueprintCallable)
	void AddStamina(float StaminaToAdd);

	UFUNCTION(BlueprintCallable)
	void AddMana(float ManaToAdd);

	UFUNCTION(BlueprintCallable)
	void RemoveHealth(float HealthToRemove);

	UFUNCTION(BlueprintCallable)
	void RemoveStamina(float StaminaToRemove);

	UFUNCTION(BlueprintCallable)
	void RemoveMana(float ManaToRemove);
};
