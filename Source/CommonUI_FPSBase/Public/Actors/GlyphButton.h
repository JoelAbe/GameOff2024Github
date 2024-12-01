// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "GlyphButton.generated.h"

class AJamPlayerCharacter;
class UStaticMeshComponent;

UCLASS()
class COMMONUI_FPSBASE_API AGlyphButton : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlyphButton();

	// Will be used to set a reference to the player character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	AJamPlayerCharacter* PlayerCharacter;

	// Allows us to create a static mesh component on the actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	UStaticMeshComponent* StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Returns the button number extracted from the actor's name
	UFUNCTION(BlueprintPure)
	int32 GetButtonNumber() const;

	// Function called when the player stops looking at this actor
	void UnHighlightMesh();

	// timer handle to allow us to set a timer regarding highlighting/unhighlighting the static mesh
	FTimerHandle UnHighlightTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Implement LookAt and Interact from the interface
	virtual void LookAt_Implementation() override;
	virtual void Interact_Implementation() override;

};
