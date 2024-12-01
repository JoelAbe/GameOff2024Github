// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "InteractActor.generated.h"

class AJamPlayerCharacter;
class UStaticMeshComponent;

UCLASS()
class COMMONUI_FPSBASE_API AInteractActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractActor();

	// Will be used to set a reference to the player character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	AJamPlayerCharacter* PlayerCharacter;

	// Allows us to create a static mesh component on the actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	UStaticMeshComponent* StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function called when the player stops looking at this actor
	void UnHighlightMesh();

	// timer handle to allow us to set a timer regarding highlighting/unhighlighting the static mesh
	FTimerHandle UnHighlightTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interface function implementations
	virtual void Interact_Implementation() override;
	virtual void LookAt_Implementation() override;



};
