// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/SaveGameStructs.h"
#include "SpellLock.generated.h"

UCLASS()
class COMMONUI_FPSBASE_API ASpellLock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellLock();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isActivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Index;

	UFUNCTION(BlueprintCallable)
	void SetStateFromSaveData(const FWorldSave& WorldSaveData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
