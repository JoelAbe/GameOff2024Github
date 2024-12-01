// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/SpellInventoryStructs.h"
#include "MagicProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class USceneComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class ANPCBaseCharacter;

UCLASS()
class COMMONUI_FPSBASE_API AMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UNiagaraComponent* ProjectileParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UNiagaraSystem* OnHitParticleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	FSpell SpellData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	bool DestroyOnHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	AActor* HitActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool DamageOnOverlap;

	TArray<ANPCBaseCharacter*> OverlappedNPCsArray;
	TArray<ANPCBaseCharacter*> DamagedNPCsArray;

//////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void DestroyProjectile();

	UFUNCTION(BlueprintImplementableEvent)
	void OnProjectileHit();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLaunch();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile();
};
