// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JamPlayerCharacter.h"

#include "Actors/Painting.h"
#include "Interfaces/InteractInterface.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/JamPlayerController.h"
#include "Player/JamPlayerState.h"
#include "Components/AttributesComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AJamPlayerCharacter::AJamPlayerCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(GetMesh());

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");

	InteractDistance = 250.f;
	ManaChargeRate = 0.2f;
	HealthChargeRate = 0.2f;
	ManaChargeAmountPerTick = 0.1;
	HealthChargeRate = 0.1;
	MaxDashCharges = 1;
	DashCharges = MaxDashCharges;
	isDashing = false;
	DashCooldownDuration = 2.0f;
	DashDistance = 600.0f;
	DashInterpSpeed = 20.0f;

	EnemyChannel = ECC_GameTraceChannel3;
}

void AJamPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Run the interact trace every frame. This will tell objects to run their look at functions if they implement the Interact interface.
	InteractTrace();

	if (isDashing)
	{
		HandleDashInterpolation(DeltaTime);
	}
}

void AJamPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AJamPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerStateRef = Cast<AJamPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	Attributes = PlayerStateRef->GetCharacterAttributes();

	OnTakeAnyDamage.AddDynamic(this, &AJamPlayerCharacter::TakeDamage);

	
}

void AJamPlayerCharacter::InteractTrace()
{
	// To do the line trace, we're gonna have to setup some local variables to keep things clean.
	// These are the vectors that will inform the line trace
	FVector CameraLocation = Camera->GetComponentLocation();
	FVector CameraForwardVector = Camera->GetForwardVector();
	FVector TraceEnd = ((CameraForwardVector * InteractDistance) + CameraLocation);

	// These variables give the line trace a place to store hit information
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// Do the actual line trace and store the hit result in bHit
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,			// Out hit result
		CameraLocation,		// Start location
		TraceEnd,			// End location
		ECC_Visibility,		// Collision channel
		CollisionParams		// Collision Parameters
	);

	// If the line trace hit something:
	if (bHit && HitResult.IsValidBlockingHit() && HitResult.GetActor())
	{
		// Get the actor we hit
		AActor* HitActor = HitResult.GetActor();

		if (HitActor != LookAtActor)
		{
			// Check if that actor implements the Interact interface, and if it does, call its interact function.
			if (HitActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				LookAtActor = HitActor;
				
				if (HitActor->IsA(APainting::StaticClass()))
				{
					InteractDistance = 500.f; // Custom distance for paintings
				}
				else
				{
					InteractDistance = 250.f; // Default distance
				}
				
				IInteractInterface::Execute_LookAt(HitActor);
			}
			else
			{
				LookAtActor = nullptr;
			}
		}
	}
	else
	{
		LookAtActor = nullptr;
	}
}

void AJamPlayerCharacter::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamagedActor != this) return;

	Attributes->RemoveHealth(Damage);
	OnDamageReceived(); // Call the blueprint implementable event

	if (Attributes->GetCurrentHealth() <= 0)
	{
		OnDeath(); // Call the blueprint implementable event
	}
}

FPlayerSave AJamPlayerCharacter::GetCharacterSaveData()
{
	FPlayerSave PlayerData;
	PlayerData.Transform = GetActorTransform();
	PlayerData.ControlRotation = GetControlRotation();
	PlayerData.Health = Attributes->GetCurrentHealth();
	PlayerData.Mana = Attributes->GetCurrentMana();
	PlayerData.SpellInventory = InventoryComponent->Inventory;
	PlayerData.EquippedSpell = PlayerStateRef->EquippedSpell;
	PlayerData.hasBlackPotion = hasBlackPotion;
	PlayerData.hasWhitePotion = hasWhitePotion;
	PlayerData.hasRedPotion = hasRedPotion;

	return PlayerData;
}

void AJamPlayerCharacter::SetPlayerStateFromSaveData(const FPlayerSave& PlayerSaveData)
{
	if (PlayerSaveData.Transform.GetLocation() == FVector::ZeroVector) return;

	SetActorTransform(PlayerSaveData.Transform);
	PlayerController->SetControlRotation(PlayerSaveData.ControlRotation);
	Attributes->Health = PlayerSaveData.Health;
	Attributes->Mana = PlayerSaveData.Mana;
	InventoryComponent->Inventory = PlayerSaveData.SpellInventory;
	PlayerStateRef->EquippedSpell = PlayerSaveData.EquippedSpell;
	hasBlackPotion = PlayerSaveData.hasBlackPotion;
	hasWhitePotion = PlayerSaveData.hasWhitePotion;
	hasRedPotion = PlayerSaveData.hasRedPotion;
	
	OnPlayerDataLoaded(); // Call the blueprint implementable event
}

void AJamPlayerCharacter::StartDash()
{
	if (DashCharges > 0 && !isDashing)
	{
		DashCharges--;
		PerformDash();
		UE_LOG(LogTemp, Warning, TEXT("DashCharges: %d"), DashCharges);

		if (!GetWorldTimerManager().IsTimerActive(FirstDashCooldownTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(FirstDashCooldownTimerHandle, this, &AJamPlayerCharacter::HandleDashCooldown, DashCooldownDuration, false);
		}
		
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
}

void AJamPlayerCharacter::PerformDash()
{
	isDashing = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(EnemyChannel, ECR_Ignore);

	// Get the input vector or fallback to forward vector
	FVector InputVector = GetCharacterMovement()->GetLastInputVector();
	FVector DashDirection = InputVector.IsNearlyZero() ? GetActorForwardVector() : InputVector.GetSafeNormal();

	// Calculate target location
	DashTargetLocation = GetActorLocation() + (DashDirection * DashDistance);

	// Ensure safe movement with interpolation and collision
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// Using Object Query to filter out specific object types
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic); // Allow static objects
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic); // Allow dynamic objects
	ObjectQueryParams.RemoveObjectTypesToQuery(ECC_Pawn); // Ignore pawns
	ObjectQueryParams.RemoveObjectTypesToQuery(EnemyChannel); // Ignore enemies
	
	// Perform line trace
	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		GetActorLocation(),
		DashTargetLocation,
		ObjectQueryParams,
		QueryParams
	);
	
	if (HitResult.bBlockingHit)
	{
		DashTargetLocation = HitResult.Location; // Prevents getting stuck in walls/enemies
	}

	// End dash after completing the interpolation
	FTimerHandle DashEndTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DashEndTimerHandle, [this]()
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			GetCapsuleComponent()->SetCollisionResponseToChannel(EnemyChannel, ECR_Block);
		
			isDashing = false;
		}, 0.1f, false);
}

void AJamPlayerCharacter::ReplenishDashCharge()
{
	
}

void AJamPlayerCharacter::HandleDashInterpolation(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, DashTargetLocation, DeltaTime, DashInterpSpeed);
	//NewLocation.Z = CurrentLocation.Z;
	SetActorLocation(NewLocation, true);
}

void AJamPlayerCharacter::HandleDashCooldown()
{
	if (DashCharges < MaxDashCharges)
	{
		DashCharges++;
		UE_LOG(LogTemp, Warning, TEXT("DashCharges: %d"), DashCharges);
	}
}
