// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PatrolRoute.h"
#include "EnemyAI.generated.h"

UENUM(BlueprintType)
enum E_MovementSpeed {
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Jogging UMETA(DisplayName = "Jogging"),
	Running UMETA(DisplayName = "Running"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyAI : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHOOTERPROJECT_API IEnemyAI
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float SetMovementSpeed(E_MovementSpeed Speed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetIdealRange(float& AttackRadius, float& DefendRadius);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EquipWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnequipWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Attack(UAnimMontage* MontageToPlay, bool UseDefaultAttackMontage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnterCrouchedState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExitCrouchedState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	APatrolRoute* GetPatrolRoute();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ReleasePatrolRoute();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FindAndSetPatrolRoute();
};
