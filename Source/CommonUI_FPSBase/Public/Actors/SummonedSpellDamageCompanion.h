// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SummonedSpellDamageCompanion.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class COMMONUI_FPSBASE_API ASummonedSpellDamageCompanion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASummonedSpellDamageCompanion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	float InitialDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	float DamagePerTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	float TickRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	float InitialDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	float EffectDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	int SpellID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	bool ShouldSlowEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UNiagaraSystem* SpawnEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UNiagaraComponent* PoolEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	TArray<AActor*> OverlappingActors;

	// Determines whether or not this actor should spawn a lingering damage pool. If true, will require further setup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	bool SpawnsDamagePool;

	FTimerHandle DamageTimerHandle;
	FTimerHandle InitialDelayTimerHandle;
	FTimerHandle EffectEndTimerHandle;
	FTimerHandle DelayBeforeDestroyTimerHandle;

////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable)
	void DealDamage();

	UFUNCTION(BlueprintCallable)
	void StartDOTEffect();

	UFUNCTION(BlueprintCallable)
	void FinishDOTEffect();

	UFUNCTION(BlueprintCallable)
	void DestroyActor();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
