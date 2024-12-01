// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AttributeItemPickup.h"
#include "Components/SphereComponent.h"
#include "Player/JamPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributesComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AAttributeItemPickup::AAttributeItemPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	SphereComponent->SetupAttachment(StaticMesh);	

	isHealthPickup = true;
	AttributeValueToAdd = 10.f;
}

// Called when the game starts or when spawned
void AAttributeItemPickup::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAttributeItemPickup::OnOverlapBegin);

	PlayerCharacter = Cast<AJamPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Called every frame
void AAttributeItemPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AAttributeItemPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!PlayerCharacter) return;
	if (OtherActor != PlayerCharacter) return;
	
	if (isHealthPickup)
	{
		if (PlayerCharacter->Attributes->Health >= PlayerCharacter->Attributes->MaxHealth) return; // if the player is full health, don't pickup the item

		PlayerCharacter->Attributes->AddHealth(AttributeValueToAdd);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation());

		OnPickup();

		Destroy();
	}
	else
	{
		if (PlayerCharacter->Attributes->Mana >= PlayerCharacter->Attributes->MaxMana) return; // if the player is full mana, don't pickup the item

		PlayerCharacter->Attributes->AddMana(AttributeValueToAdd);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation());

		OnPickup();

		Destroy();
	}
}

