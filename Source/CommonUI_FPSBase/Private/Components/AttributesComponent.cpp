// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize variables
	Health = 100.f;
	MaxHealth = 100.f;
	Mana = 100.f;
	MaxMana = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
}


void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributesComponent::InitializeAttributeValues()
{
	Health = MaxHealth;
	Mana = MaxMana;
	Stamina = MaxStamina;

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("Initialized Attribute values"));
}

float UAttributesComponent::GetCurrentHealth()
{
	return Health;
}

float UAttributesComponent::GetCurrentMana()
{
	return Mana;
}

float UAttributesComponent::GetCurrentStamina()
{
	return Stamina;
}

void UAttributesComponent::AddHealth(float HealthToAdd)
{
	Health = FMath::Clamp(Health + HealthToAdd, 0, MaxHealth);
}

void UAttributesComponent::AddStamina(float StaminaToAdd)
{
	Stamina = FMath::Clamp(Stamina + StaminaToAdd, 0, MaxStamina);
}

void UAttributesComponent::AddMana(float ManaToAdd)
{
	Mana = FMath::Clamp(Mana + ManaToAdd, 0, MaxMana);
}

void UAttributesComponent::RemoveHealth(float HealthToRemove)
{
	Health = FMath::Clamp(Health - HealthToRemove, 0, MaxHealth);
}

void UAttributesComponent::RemoveStamina(float StaminaToRemove)
{
	Stamina = FMath::Clamp(Stamina - StaminaToRemove, 0, MaxStamina);
}

void UAttributesComponent::RemoveMana(float ManaToRemove)
{
	Mana = FMath::Clamp(Mana - ManaToRemove, 0, MaxMana);
}
