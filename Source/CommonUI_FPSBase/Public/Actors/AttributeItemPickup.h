// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttributeItemPickup.generated.h"

class USphereComponent;
class AJamPlayerCharacter;
class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class COMMONUI_FPSBASE_API AAttributeItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttributeItemPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UNiagaraSystem* PickupEffect;

	// If false, item will change to Mana pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool isHealthPickup;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AJamPlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttributeValueToAdd;


	///////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickup();
};
