// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/SpellInventoryStructs.h"
#include "InventoryComponent.generated.h"

class AJamPlayerController;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMMONUI_FPSBASE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FSpell EmptySpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FSpell> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	AJamPlayerController* PlayerControllerRef;

public:	
	UFUNCTION(BlueprintCallable)
	void Add(const FSpell& SpellData);

	UFUNCTION(BlueprintCallable)
	void Remove(const FSpell& SpellData);

	UFUNCTION(BlueprintCallable)
	bool FindEmptySlot(int32& OutSlotIndex);
};
