// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CombatTrigger.h"

// Sets default values
ACombatTrigger::ACombatTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatComplete = false;
	Index = -1;
}

// Called when the game starts or when spawned
void ACombatTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombatTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatTrigger::SetStateFromSaveData(const FWorldSave& WorldSaveData)
{
	if (Index != -1)
	{
		if (WorldSaveData.CombatTriggersArray.Num() > 0)
		{
			CombatComplete = WorldSaveData.CombatTriggersArray[Index];
		}
	}
}

