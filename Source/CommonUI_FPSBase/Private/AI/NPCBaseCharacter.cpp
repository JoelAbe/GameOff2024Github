// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPCBaseCharacter.h"
#include "Components/AttributesComponent.h"
#include "Actors/CombatManager.h"
#include "Kismet/GameplayStatics.h"
#include "AI/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/JamPlayerCharacter.h"
#include "Actors/MagicProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Gameframework/Character.h"
#include "Gameframework/CharacterMovementComponent.h"

ANPCBaseCharacter::ANPCBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterAttributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Character Attributes"));

	// Initialize variables
	isRanged = true;
	hasCombatToken = false;
	BaseDamage = 10.f;
	hasSuperArmor = false;
	isDead = false;
	DamageRequiredToBreakSuperArmor = 50.f;
	TimeToBreakSuperArmor = 5.f;
	AccumulatedDamage = 0.f;
	DefaultWalkSpeed = 400.f;
	isSlowed = false;
}

void ANPCBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetReferences();

	if (AIController && PlayerCharacter)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(FName("AttackTarget"), PlayerCharacter);
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("AI Controller, blackboard component, and player character set"));
	}

	OnTakeAnyDamage.AddDynamic(this, &ANPCBaseCharacter::TakeDamage);
}

void ANPCBaseCharacter::SetReferences()
{
	AActor* FoundCombatManager = UGameplayStatics::GetActorOfClass(this, ACombatManager::StaticClass());
	CombatManager = Cast<ACombatManager>(FoundCombatManager);

	AIController = Cast<AAIControllerBase>(GetController());

	PlayerCharacter = Cast<AJamPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

void ANPCBaseCharacter::RequestToken()
{
	if (!CombatManager) return;
	if (hasCombatToken) return;

	CombatManager->ManageTokenRequest(this);
}

void ANPCBaseCharacter::OnTokenRequestApproved()
{
	hasCombatToken = true;

	if (!AIController) return;
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("CanAttack"), true);
}

void ANPCBaseCharacter::OnTokenRequestDenied()
{
	hasCombatToken = false;

	if (!AIController) return;
	AIController->GetBlackboardComponent()->SetValueAsBool(FName("CanAttack"), false);
}

void ANPCBaseCharacter::StartAttack()
{
	SetStateAttacking();
}

void ANPCBaseCharacter::SetStateAttacking()
{
	uint8 Attacking = static_cast<uint8>(ECombatState::Attacking);

	CombatState = ECombatState::Attacking;
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName("CombatState"), Attacking);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("AI Controller and blackboard component NOT set"));
	}
}

void ANPCBaseCharacter::SetStateMoving()
{
	uint8 Moving = static_cast<uint8>(ECombatState::Moving);

	CombatState = ECombatState::Moving;
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName("CombatState"), Moving);
	}
}

void ANPCBaseCharacter::SetStateTaunting()
{
	uint8 Taunting = static_cast<uint8>(ECombatState::Taunting);

	CombatState = ECombatState::Taunting;
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName("CombatState"), Taunting);
	}

	OnTaunt(); // Call the blueprint implementable event
}

void ANPCBaseCharacter::SetStateReadyToAttack()
{
	uint8 ReadyToAttack = static_cast<uint8>(ECombatState::ReadyToAttack);

	CombatState = ECombatState::ReadyToAttack;
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName("CombatState"), ReadyToAttack);
	}
}

void ANPCBaseCharacter::SetStateWaiting()
{
	uint8 Waiting = static_cast<uint8>(ECombatState::Waiting);

	CombatState = ECombatState::Waiting;
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName("CombatState"), Waiting);
	}
}

void ANPCBaseCharacter::SetStateDead()
{
	uint8 Dead = static_cast<uint8>(ECombatState::Dead);

	CombatState = ECombatState::Dead;
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName("CombatState"), Dead);
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("CanAttack"), false);
	}
}

void ANPCBaseCharacter::SetStateStunned()
{
	if (hasCombatToken)
	{
		OnTokenRequestDenied();
		CombatManager->PutTokenOnCooldown(ClassType);
	}

	uint8 Stunned = static_cast<uint8>(ECombatState::Stunned);

	CombatState = ECombatState::Stunned;
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName("CombatState"), Stunned);
	}
}

void ANPCBaseCharacter::FinishAttacking()
{
	CombatManager->PutTokenOnCooldown(ClassType);
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("AI called FinishAttacking()"));

	hasCombatToken = false;

	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(FName("CanAttack"), false);
	}

	if (CanSeePlayer() && isRanged)
	{
		SetStateReadyToAttack();
	}
	else
	{
		AssignNextAction();
	}
}

void ANPCBaseCharacter::AssignNextAction()
{
	if (isRanged) 
	{
		int RandomChoice = UKismetMathLibrary::RandomIntegerInRange(1, 5);
		if (RandomChoice == 1 && CombatState != ECombatState::Waiting)
		{
			SetStateWaiting();
		}
		else if (RandomChoice == 2 && CombatState != ECombatState::Taunting)
		{
			SetStateTaunting();
		}
		else
		{
			SetStateMoving();
		}
	}
	else
	{
		SetStateMoving();
	}
}

void ANPCBaseCharacter::Die()
{
	if (isDead) return;

	isDead = true;

	if (hasCombatToken)
	{
		OnTokenRequestDenied();
		CombatManager->PutTokenOnCooldown(ClassType);
	}

	OnNPCDeath.Broadcast();
	SetStateDead();
	OnDeath(); // call the blueprint implementable event
}

void ANPCBaseCharacter::TryStun()
{
	if (hasSuperArmor || CombatState == ECombatState::Stunned || CanBeStunned) return;

	int RandomChoice = UKismetMathLibrary::RandomIntegerInRange(1, 10);

	switch (ClassType)
	{
	case EClassType::Light:
		if (RandomChoice <= 5)
		{
			SetStateStunned();
			StartStunCooldown();
		}
		break;
	case EClassType::Medium:
		if (RandomChoice <= 2)
		{
			SetStateStunned();
			StartStunCooldown();
		}
		break;
	case EClassType::Heavy:
		if (RandomChoice == 1)
		{
			SetStateStunned();
			StartStunCooldown();
		}
		break;
	}

}

void ANPCBaseCharacter::ResetAccumulatedDamage()
{
	AccumulatedDamage = 0.f;
	hasSuperArmor = true;

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Super armor DPS check not met, accumulated damage has been reset"));
}

void ANPCBaseCharacter::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamagedActor != this) return;

	// Do a DPS check - if the player can deal enough damage within the amount of time specified in TimeToBreakSuperArmor, then we have broken the super armor and have a chance to stun
	// Otherwise, reset AccumulatedDamage and let the timer clear itself. 
	if (!GetWorldTimerManager().IsTimerActive(SuperArmorTimerHandle) && hasSuperArmor)
	{
		GetWorld()->GetTimerManager().SetTimer(SuperArmorTimerHandle, this, &ANPCBaseCharacter::ResetAccumulatedDamage, TimeToBreakSuperArmor);

		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("New super armor DPS check timer set"));
	}

	if (hasSuperArmor)
	{
		AccumulatedDamage += Damage;
		if (AccumulatedDamage >= DamageRequiredToBreakSuperArmor)
		{
			hasSuperArmor = false;
			CharacterAttributes->RemoveHealth(Damage);
			TryStun();
			GetWorld()->GetTimerManager().ClearTimer(SuperArmorTimerHandle);

			OnSuperArmorBroken(); // call the blueprint event so we can play WWise sound here

			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("Super armor broken!"));
		}
		else
		{
			CharacterAttributes->RemoveHealth(Damage / 2);
		}
	}
	else
	{
		// Don't have super armor, treat damage like a normal NPC
		CharacterAttributes->RemoveHealth(Damage);
		TryStun();
	}

	if (CharacterAttributes->GetCurrentHealth() <= 0)
	{
		Die();
	}

	if (CombatState != ECombatState::Attacking && CombatState != ECombatState::Dead)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
	}
	
	OnDamageReceieved(); // Call the blueprint implementable event
}

void ANPCBaseCharacter::EndSlow()
{
	if (!isSlowed) return;

	isSlowed = false;

	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetMesh()->SetPlayRate(1);
}

void ANPCBaseCharacter::StartSlow()
{
	if (isSlowed) return;

	isSlowed = true;

	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed / 2;
	GetMesh()->SetPlayRate(0.5);
}

void ANPCBaseCharacter::JumpToDestination(FVector Destination)
{
	FVector LaunchVelocity;
	Destination.Z += 400;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, LaunchVelocity, GetActorLocation(), Destination);

	LaunchCharacter(LaunchVelocity, true, true);
}

bool ANPCBaseCharacter::CanSeePlayer()
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	bool CanSeePlayer = false;

	bool Hit = GetWorld()->LineTraceSingleByChannel(
		HitResult, 
		GetActorLocation(), 
		PlayerCharacter->GetActorLocation(), 
		ECollisionChannel::ECC_Visibility, 
		QueryParams
	);

	if (Hit && HitResult.GetActor() == PlayerCharacter)
	{
		if (AIController)
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("CanSeePlayer"), true);
		}
		CanSeePlayer = true;
	}
	else
	{
		if (AIController)
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("CanSeePlayer"), false);
		}
		CanSeePlayer = false;
	}

	return CanSeePlayer;
}

void ANPCBaseCharacter::StartStunCooldown()
{
	CanBeStunned = false;

	// Set a timer to reset the flag after 2 seconds
	GetWorldTimerManager().SetTimer(StunCooldownTimer, this, &ANPCBaseCharacter::ResetStunCooldown, 2.0f, false);
}

void ANPCBaseCharacter::ResetStunCooldown()
{
	CanBeStunned = true;
}
