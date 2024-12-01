// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/UnlockableDoor.h"


// Sets default values
AUnlockableDoor::AUnlockableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isUnlocked = false;
	Index = -1;
}

void AUnlockableDoor::SetStateFromSaveData(const FWorldSave& WorldSaveData)
{
	if (Index != -1)
	{
		if (WorldSaveData.UnlockableDoorsArray.Num() > 0)
		{
			isUnlocked = WorldSaveData.UnlockableDoorsArray[Index];
		}	
	}
}

// Called when the game starts or when spawned
void AUnlockableDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnlockableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

