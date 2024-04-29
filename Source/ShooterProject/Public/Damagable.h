// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damagable.generated.h"

UENUM(BlueprintType)
enum EDamageType {
	Melee UMETA(DisplayName = "Melee"),
	Projectile UMETA(DisplayName = "Projectile"),
	Explosion UMETA(DisplayName = "Explosion"),
	Environmental UMETA(DisplayName = "Environmental"),
};

UENUM(BlueprintType)
enum EDamageResponse {
	Hit UMETA(DisplayName = "Hit"),
	Stagger UMETA(DisplayName = "Stagger"),
	Stun UMETA(DisplayName = "Stun"),
	Knockback UMETA(DisplayName = "Knockback"),
};

USTRUCT(BlueprintType)
struct FSDamageInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EDamageResponse> DamageResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldDamageInvincible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanBeBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanBeParried;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldForceInterrupt;

};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamagable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHOOTERPROJECT_API IDamagable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float Heal(float Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool TakeDamage(FSDamageInfo DamageInfo, AActor* DamageCauser);
};
