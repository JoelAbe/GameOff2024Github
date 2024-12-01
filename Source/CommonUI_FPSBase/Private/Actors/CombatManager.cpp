// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CombatManager.h"
#include "AI/NPCBaseCharacter.h"
#include "TimerManager.h"

// Sets default values
ACombatManager::ACombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightTokensAvailable = 0;
	MaxLightTokens = 3;
	LightToken = EClassType::Light;
	MediumTokensAvailable = 0;
	MaxMediumTokens = 2;
	MediumToken = EClassType::Medium;
	HeavyTokensAvailable = 0;
	MaxHeavyTokens = 2;
	HeavyToken = EClassType::Heavy;
	TokenCooldownDuration = 3.f;
}

// Called when the game starts or when spawned
void ACombatManager::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeTokenValues();
}

// Called every frame
void ACombatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatManager::InitializeTokenValues()
{
	LightTokensAvailable = MaxLightTokens;
	MediumTokensAvailable = MaxMediumTokens;
	HeavyTokensAvailable = MaxHeavyTokens;
}

int ACombatManager::GetAvailableTokensByType(EClassType ClassType)
{
	switch (ClassType)
	{
	case EClassType::Light:
		return LightTokensAvailable;
		break;
	case EClassType::Medium:
		return MediumTokensAvailable;
		break;
	case EClassType::Heavy:
		return HeavyTokensAvailable;
		break;
	}
	return -1;
}

void ACombatManager::RemoveTokenByType(EClassType ClassType)
{
	switch (ClassType)
	{
	case EClassType::Light:
		LightTokensAvailable--;
		break;
	case EClassType::Medium:
		MediumTokensAvailable--;
		break;
	case EClassType::Heavy:
		HeavyTokensAvailable--;
		break;
	}
}

void ACombatManager::TakeTokenOffCooldown()
{
	// Make tokens available based on their position in the queue

	for (TPair<EClassType, int32>& Pair : TokensOnCooldown)
	{
		if (Pair.Value > 0)
		{
			Pair.Value--;
			UE_LOG(LogTemp, Warning, TEXT("Token cooldown expired. %s now has %d tokens on cooldown."),
				*UEnum::GetValueAsString(Pair.Key), Pair.Value);

			switch (Pair.Key)
			{
			case EClassType::Light:
				LightTokensAvailable++;
				break;
			case EClassType::Medium:
				MediumTokensAvailable++;
				break;
			case EClassType::Heavy:
				HeavyTokensAvailable++;
				break;
			}

			break;
		}
	}
}

void ACombatManager::PutTokenOnCooldown(EClassType ClassType)
{
	if (TokensOnCooldown.Contains(ClassType))
	{
		TokensOnCooldown[ClassType]++;
	}
	else
	{
		TokensOnCooldown.Add(ClassType, 1);
	}

	UE_LOG(LogTemp, Warning, TEXT("Added token to cooldown. %s now has %d tokens on cooldown."),
		*UEnum::GetValueAsString(ClassType), TokensOnCooldown[ClassType]);

	FTimerHandle NewTimerHandle;
	GetWorldTimerManager().SetTimer(NewTimerHandle, this, &ThisClass::TakeTokenOffCooldown, TokenCooldownDuration, false, static_cast<float>(ClassType));
}

void ACombatManager::ManageTokenRequest(ANPCBaseCharacter* NPC)
{
	// Get the NPC class type and check how many tokens are available of that type
	EClassType NPCType = NPC->GetClassType();
	int AvailableTokens = GetAvailableTokensByType(NPCType);

	if (AvailableTokens <= 0)
	{
		NPC->OnTokenRequestDenied();
	}
	else
	{
		NPC->OnTokenRequestApproved();
		RemoveTokenByType(NPCType);
	}
}

