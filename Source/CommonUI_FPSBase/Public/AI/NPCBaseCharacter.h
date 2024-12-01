// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/AIData.h"
#include "NPCBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNPCDeath);

class UAttributesComponent;
class ACombatManager;
class UBehaviorTree;
class AAIControllerBase;
class AJamPlayerCharacter;
class AMagicProjectile;
class UNiagaraSystem;
class UAkAudioEvent;

UCLASS()
class COMMONUI_FPSBASE_API ANPCBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Variables

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnNPCDeath OnNPCDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributesComponent* CharacterAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool isRanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool hasCombatToken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EClassType ClassType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	ECombatState CombatState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	ACombatManager* CombatManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	AAIControllerBase* AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	AJamPlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AMagicProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BaseDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool hasSuperArmor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool isDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DamageRequiredToBreakSuperArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TimeToBreakSuperArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AccumulatedDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UNiagaraSystem* SuperArmorBreakEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DefaultWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool isSlowed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (ExposeOnSpawn = "true"))
	AActor* RoomOwner;

	FTimerHandle SuperArmorTimerHandle;
	FTimerHandle StunCooldownTimer;
	bool CanBeStunned = true;

	/////////////////////////////////////////////////////

	//Functions 

	void SetReferences();

	UFUNCTION(BlueprintCallable)
	void RequestToken();

	UFUNCTION(BlueprintCallable)
	void OnTokenRequestApproved();

	UFUNCTION(BlueprintCallable)
	void OnTokenRequestDenied();

	UFUNCTION(BlueprintCallable)
	void StartAttack();

	UFUNCTION(BlueprintCallable)
	void SetStateAttacking();

	UFUNCTION(BlueprintCallable)
	void SetStateMoving();

	UFUNCTION(BlueprintCallable)
	void SetStateTaunting();

	UFUNCTION(BlueprintCallable)
	void SetStateReadyToAttack();

	UFUNCTION(BlueprintCallable)
	void SetStateWaiting();

	UFUNCTION(BlueprintCallable)
	void SetStateDead();

	UFUNCTION(BlueprintCallable)
	void SetStateStunned();

	UFUNCTION(BlueprintCallable)
	void FinishAttacking();

	UFUNCTION(BlueprintCallable)
	void AssignNextAction();

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void TryStun();

	UFUNCTION(BlueprintCallable)
	void ResetAccumulatedDamage();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSuperArmorBroken();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTaunt();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageReceieved();

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void EndSlow();

	UFUNCTION(BlueprintCallable)
	void StartSlow();

	UFUNCTION(BlueprintCallable)
	void JumpToDestination(FVector Destination);

	UFUNCTION(BlueprintCallable)
	bool CanSeePlayer();

	void StartStunCooldown();
	void ResetStunCooldown();

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	FORCEINLINE EClassType GetClassType() const { return ClassType; }
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE bool HasCombatToken() const { return hasCombatToken; }
};
