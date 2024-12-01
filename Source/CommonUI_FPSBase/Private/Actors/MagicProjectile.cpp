// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MagicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/NPCBaseCharacter.h"

// Sets default values
AMagicProjectile::AMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	ProjectileMovementComponent->bAutoActivate = false;
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(GetRootComponent());
	ProjectileMesh->SetGenerateOverlapEvents(true);

	ProjectileParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile Particle"));
	ProjectileParticle->SetupAttachment(ProjectileMesh);

	BaseDamage = 10.f;
	DestroyOnHit = true;
	HitActor = nullptr;
	DamageOnOverlap = false;
}

void AMagicProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FTimerHandle DelayTimerHandle;

	HitActor = OtherActor;

	if (OnHitParticleEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, OnHitParticleEffect, Hit.ImpactPoint);
	}

	ProjectileMesh->SetHiddenInGame(true);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!DamageOnOverlap)
	{
		UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, GetInstigatorController(), this, UDamageType::StaticClass());
	}

	OnProjectileHit(); // call the blueprint event so we can play a sound here if we want

	if (DestroyOnHit)
	{
		ProjectileParticle->Deactivate();
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AMagicProjectile::DestroyProjectile, 2.f);
	}
}

void AMagicProjectile::DestroyProjectile()
{
	Destroy();
}

void AMagicProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageOnOverlap) return;

	TArray<AActor*> OverlappingActors;

	ProjectileMesh->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		ANPCBaseCharacter* NPC = Cast<ANPCBaseCharacter>(Actor);

		if (NPC)
		{
			OverlappedNPCsArray.AddUnique(NPC);
		}
	}

	for (ANPCBaseCharacter* NPC : OverlappedNPCsArray)
	{
		if (OtherActor && OtherActor != this && !DamagedNPCsArray.Contains(NPC))
		{
			UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, GetInstigatorController(), this, UDamageType::StaticClass());
			DamagedNPCsArray.AddUnique(NPC);
		}
	}
}

// Called when the game starts or when spawned
void AMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AMagicProjectile::OnHit);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AMagicProjectile::OnOverlapBegin);
}

void AMagicProjectile::LaunchProjectile()
{
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->Activate();
		OnLaunch(); // Call the blueprint implementable event
	}
}


