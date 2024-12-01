// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SpellLock.h"

// Sets default values
ASpellLock::ASpellLock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isActivated = false;
	Index = -1;
}

void ASpellLock::SetStateFromSaveData(const FWorldSave& WorldSaveData)
{
	if (Index != -1)
	{
		if (WorldSaveData.SpellLocksArray.Num() > 0)
		{
			isActivated = WorldSaveData.SpellLocksArray[Index];
		}
	}
}

// Called when the game starts or when spawned
void ASpellLock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpellLock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

