// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Checkpoint.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/SpellLock.h"
#include "Actors/UnlockableDoor.h"
#include "Actors/TranslatableGlyph.h"
#include "Actors/CombatTrigger.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FWorldSave ACheckpoint::GetWorldSaveData()
{
	// Empty local variables to store data in
	TArray<AActor*> FoundSpellLockActors;
	TArray<AActor*> FoundDoorActors;
	TArray<AActor*> FoundCombatTriggers;
	TArray<AActor*> FoundGlyphs;
	TArray<bool> SpellLocks;
	TArray<bool> Doors;
	TArray<bool> CombatTriggers;
	TArray<bool> Glyphs;
	FWorldSave WorldSave;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpellLock::StaticClass(), FoundSpellLockActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnlockableDoor::StaticClass(), FoundDoorActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACombatTrigger::StaticClass(), FoundCombatTriggers);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATranslatableGlyph::StaticClass(), FoundGlyphs);

	SpellLocks.Init(false, FoundSpellLockActors.Num());
	Doors.Init(false, FoundDoorActors.Num());
	CombatTriggers.Init(false, FoundCombatTriggers.Num());
	Glyphs.Init(false, FoundGlyphs.Num());

	for (AActor* Actor : FoundSpellLockActors)
	{
		ASpellLock* SpellLock = Cast<ASpellLock>(Actor);

		// Ensure there are no spell locks with invalid indices
		if (!SpellLock || SpellLock->Index < 0 || SpellLock->Index > FoundSpellLockActors.Num())
		{
			FString Message = FString::Printf(TEXT("Crystal with invalid data: %d"), SpellLock->Index);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, *Message);

			return FWorldSave();
		}

		SpellLocks[SpellLock->Index] = SpellLock->isActivated;
	}

	for (AActor* Actor : FoundDoorActors)
	{
		AUnlockableDoor* Door = Cast<AUnlockableDoor>(Actor);

		if (!Door || Door->Index < 0 || Door->Index > FoundDoorActors.Num())
		{
			FString Message = FString::Printf(TEXT("Door with invalid data: %d"), Door->Index);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, *Message);

			return FWorldSave();
		}

		Doors[Door->Index] = Door->isUnlocked;
	}

	for (AActor* Actor : FoundCombatTriggers)
	{
		ACombatTrigger* CombatTrigger = Cast<ACombatTrigger>(Actor);

		// Ensure there are no spell locks with invalid indices
		if (!CombatTrigger || CombatTrigger->Index < 0 || CombatTrigger->Index > FoundCombatTriggers.Num())
		{
			FString Message = FString::Printf(TEXT("Crystal with invalid data: %d"), CombatTrigger->Index);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, *Message);

			return FWorldSave();
		}

		CombatTriggers[CombatTrigger->Index] = CombatTrigger->CombatComplete;
	}

	for (AActor* Actor : FoundGlyphs)
	{
		ATranslatableGlyph* Glyph = Cast<ATranslatableGlyph>(Actor);

		// Ensure there are no spell locks with invalid indices
		if (!Glyph || Glyph->Index < 0 || Glyph->Index > FoundGlyphs.Num())
		{
			FString Message = FString::Printf(TEXT("Crystal with invalid data: %d"), Glyph->Index);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, *Message);

			return FWorldSave();
		}

		Glyphs[Glyph->Index] = Glyph->isTranslated;
	}

	WorldSave.SpellLocksArray = SpellLocks;
	WorldSave.UnlockableDoorsArray = Doors;
	WorldSave.CombatTriggersArray = CombatTriggers;
	WorldSave.GlyphsArray = Glyphs;

	return WorldSave;
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

