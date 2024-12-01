// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/JamPlayerController.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<AJamPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
}

void UInventoryComponent::Add(const FSpell& SpellData)
{
	// Create a local variable that FindEmptySlot can store the found index in:
	int32 EmptySlotIndex;
	bool SlotFound = FindEmptySlot(EmptySlotIndex);

	if (SlotFound)
	{
		Inventory[EmptySlotIndex] = SpellData;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Added spell to slot: %d"), EmptySlotIndex));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("No more availabe inventory slots to add this spell to!"));
	}
}

void UInventoryComponent::Remove(const FSpell& SpellData)
{
	// We probably don't need this function but it's ready to implement if we do
}

bool UInventoryComponent::FindEmptySlot(int32& OutSlotIndex)
{
	bool EmptySlotFound = false;
	int SlotIndex = 0;
	for (FSpell& Spell : Inventory)
	{
		if (Spell.isValid)
		{
			EmptySlotFound = false;
			OutSlotIndex = -1;
		}
		else
		{
			EmptySlotFound = true;
			OutSlotIndex = SlotIndex;
			break;
		}
		SlotIndex++;
	}
	return EmptySlotFound;
}