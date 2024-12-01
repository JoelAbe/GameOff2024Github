// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractActor.h"
#include "Player/JamPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AInteractActor::AInteractActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the static mesh component and attach it to the root component
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AInteractActor::BeginPlay()
{
	Super::BeginPlay();

	// Get/set a reference to the player character when the game starts
	PlayerCharacter = Cast<AJamPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void AInteractActor::UnHighlightMesh()
{
	// If the actor the player is looking at ISN'T this actor:
	if (PlayerCharacter->LookAtActor != this)
	{
		// Disable highlight and clear the highlight timer
		StaticMesh->SetRenderCustomDepth(false);
		GetWorldTimerManager().ClearTimer(UnHighlightTimerHandle);
	}
}

// Called every frame
void AInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractActor::Interact_Implementation()
{
	// Implement this function in blueprint or in child classes as it will probably be different for every actor
}

void AInteractActor::LookAt_Implementation()
{
	// if the player character reference isn't valid, return
	if (!PlayerCharacter) return;

	// bool to make sure we don't repeatedly set the custom depth stencil value to 250. this only needs to run once so we'll set a bool to ensure that happens
	bool hasSetStencilValue = false;

	// Allow the item to be highlighted, then set a timer that runs on tick to check if the player stopped looking at this actor.
	StaticMesh->SetRenderCustomDepth(true);
	GetWorldTimerManager().SetTimer(UnHighlightTimerHandle, this, &AInteractActor::UnHighlightMesh, GetWorld()->GetDeltaSeconds(), true);
	
	// Finally, if we haven't already set the stencil value, set it.
	if (!hasSetStencilValue)
	{
		StaticMesh->SetCustomDepthStencilValue(250);
	}

}

