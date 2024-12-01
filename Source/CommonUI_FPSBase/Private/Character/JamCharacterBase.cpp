// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/JamCharacterBase.h"
#include "Camera/CameraComponent.h"

// Sets default values
AJamCharacterBase::AJamCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJamCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJamCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}