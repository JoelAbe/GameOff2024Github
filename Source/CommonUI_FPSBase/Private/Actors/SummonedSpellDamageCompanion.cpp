// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SummonedSpellDamageCompanion.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Actors/MagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/NPCBaseCharacter.h"

// Sets default values
ASummonedSpellDamageCompanion::ASummonedSpellDamageCompanion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	CollisionSphere->SetupAttachment(GetRootComponent());

	PoolEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Pool Effect"));
	PoolEffect->SetupAttachment(CollisionSphere);
	PoolEffect->bAutoActivate = false;

	InitialDamage = 10.f;
	DamagePerTick = 1.f;
	TickRate = 0.2f;
	SpawnsDamagePool = false;
	EffectDuration = 5.f;
	ShouldSlowEnemies = false;
	SpellID = -1;
}

// Called when the game starts or when spawned
void ASummonedSpellDamageCompanion::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASummonedSpellDamageCompanion::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ASummonedSpellDamageCompanion::OnOverlapEnd);
	
	// After the initial delay (this timer), apply damage to all the actors currently overlapping the collision sphere
	GetWorld()->GetTimerManager().SetTimer(InitialDelayTimerHandle, this, &ASummonedSpellDamageCompanion::DealDamage, InitialDelay, false);

	if (SpawnEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SpawnEffect, GetActorLocation());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("You didn't set the spawn effect on this companion actor."));
	}
}

// Called every frame
void ASummonedSpellDamageCompanion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASummonedSpellDamageCompanion::DealDamage()
{
	CollisionSphere->GetOverlappingActors(OverlappingActors, ANPCBaseCharacter::StaticClass());
	for (AActor* Actor : OverlappingActors)
	{
		UGameplayStatics::ApplyDamage(Actor, InitialDamage, UGameplayStatics::GetPlayerController(this, 0), this, UDamageType::StaticClass());

		if (ShouldSlowEnemies)
		{
			ANPCBaseCharacter* NPC = Cast<ANPCBaseCharacter>(Actor);
			if (NPC)
			{
				NPC->StartSlow();
			}
		}
	}
	
	if (!SpawnsDamagePool)
	{
		// In 2 seconds, Destroy()
		GetWorld()->GetTimerManager().SetTimer(DelayBeforeDestroyTimerHandle, this, &ASummonedSpellDamageCompanion::DestroyActor, 2.f, false);
		return;
	}

	// Looping timer that actually applies the damage each tick (TickRate)
	GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &ASummonedSpellDamageCompanion::StartDOTEffect, TickRate, true);

	// Simply acts as a delay - stops the DOT effect after [EffectDuration] seconds
	GetWorld()->GetTimerManager().SetTimer(EffectEndTimerHandle, this, &ASummonedSpellDamageCompanion::FinishDOTEffect, EffectDuration, false);

	// Activate the pool effect
	if (PoolEffect)
	{
		PoolEffect->Activate();
	}
	
}

void ASummonedSpellDamageCompanion::StartDOTEffect()
{
	for (AActor* Actor : OverlappingActors)
	{
		UGameplayStatics::ApplyDamage(Actor, DamagePerTick, UGameplayStatics::GetPlayerController(this, 0), this, UDamageType::StaticClass());
;
		if (ShouldSlowEnemies)
		{
			// Need to call StartSlow() again for any actors that walk into the effect after the initial burst of damage
			ANPCBaseCharacter* NPC = Cast<ANPCBaseCharacter>(Actor);
			NPC->StartSlow();
		}
	}
}

void ASummonedSpellDamageCompanion::FinishDOTEffect()
{
	if (ShouldSlowEnemies)
	{
		for (AActor* Actor : OverlappingActors)
		{
			ANPCBaseCharacter* NPC = Cast<ANPCBaseCharacter>(Actor);
			NPC->EndSlow();
		}
	}

	if (PoolEffect)
	{
		PoolEffect->Deactivate();
	}

	// Stop the tick timer that applies damage
	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);

	// In 2 seconds, Destroy()
	GetWorld()->GetTimerManager().SetTimer(DelayBeforeDestroyTimerHandle, this, &ASummonedSpellDamageCompanion::DestroyActor, 2.f, false);
}

void ASummonedSpellDamageCompanion::DestroyActor()
{
	Destroy();
}

void ASummonedSpellDamageCompanion::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANPCBaseCharacter* NPC = Cast<ANPCBaseCharacter>(OtherActor);

	if (NPC && SpawnsDamagePool)
	{
		OverlappingActors.AddUnique(NPC);
	}
}

void ASummonedSpellDamageCompanion::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ANPCBaseCharacter* NPC = Cast<ANPCBaseCharacter>(OtherActor);

	if (NPC && SpawnsDamagePool)
	{
		OverlappingActors.Remove(NPC);
		NPC->EndSlow();
	}
}

