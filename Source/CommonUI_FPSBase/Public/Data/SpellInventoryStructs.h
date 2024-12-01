// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpellInventoryStructs.generated.h"

class UNiagaraSystem;
class AMagicProjectile;
class UAkAudioEvent;
class ASummonedSpellDamageCompanion;

UENUM(BlueprintType)
enum class ESpellType : uint8
{
	TwoHanded	UMETA(DisplayName = "Two-Handed"),
	Beam		UMETA(DisplayName = "Beam"),
	Projectile	UMETA(DisplayName = "Projectile"),
	Conjure		UMETA(DisplayName = "Conjure")
};

USTRUCT(BlueprintType)
struct FSpell : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	ESpellType SpellType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool isValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool isTwoHanded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ChargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UNiagaraSystem* IdleParticleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AMagicProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UAkAudioEvent* SpellIdleSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UAkAudioEvent* SpellChargeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UAkAudioEvent* SpellFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FVector SpellDecalSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<ASummonedSpellDamageCompanion> DamageCompanion;

	// Initialize the values
	FSpell() :
		ID(0),
		Name(NAME_None),
		SpellType(ESpellType::Projectile),
		Icon(nullptr),
		Description(FText::GetEmpty()),
		isValid(false),
		isTwoHanded(false),
		ChargeTime(0.f),
		IdleParticleEffect(nullptr),
		ProjectileClass(nullptr),
		BaseDamage(0.f),
		ManaCost(1.f),
		SpellIdleSound(nullptr),
		SpellChargeSound(nullptr),
		SpellFireSound(nullptr),
		SpellDecalSize(FVector(1.f, 1.f, 1.f))
	{}
};

/**
 * 
 */
UCLASS()
class COMMONUI_FPSBASE_API USpellInventoryStructs : public UObject
{
	GENERATED_BODY()
	
};
