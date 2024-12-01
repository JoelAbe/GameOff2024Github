// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JamPlayerState.h"
#include "Components/AttributesComponent.h"

AJamPlayerState::AJamPlayerState()
{
	CharacterAttributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes Component"));
}
