// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Delegates/Delegate.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIEnemyBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EState {
	Passive UMETA(DisplayName = "Passive"),
	Attacking UMETA(DisplayName = "Attacking"),
	Frozen UMETA(DisplayName = "Frozen"),
	Patrolling UMETA(DisplayName = "Patrolling"),
	Investigating UMETA(DisplayName = "Investigating"),
	Dead UMETA(DisplayName = "Dead"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHostileDetected, AActor*, Hostile);
	
UCLASS()
class SHOOTERPROJECT_API AAIEnemyBase : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	class AActor* AttackTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ShooterProject")
	UBlackboardData* BlackboardToUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	UBlackboardComponent* BlackboardBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	FName AttackTargetKeyName = FName("AttackTarget");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	FName AttackRadiusKeyName = FName("AttackRadius");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	FName StateKeyName = FName("State");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	FName PointOfInterestKeyName = FName("PointOfInterest");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	FName DefendRadiusKeyName = FName("DefendRadius");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	FName SpawnPointKeyName = FName("SpawnPointLocation");

	virtual void OnPossess(APawn* PossessedPawn) override;

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	EState GetCurrentState();

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void SetStateAsPassive();
	
	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void SetStateAsAttacking(AActor* Target, bool UsePreviousKnownAttackTarget);

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void SetStateAsInvestigating(FVector Location);
	
	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void SetStateAsFrozen();

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void SetStateAsDead();

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void SetStateAsPatrolling();

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void SetSpawnPointLocation(FVector Location);

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void HandleSensedSight(AActor* Hostile);

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void HandleSensedDamage(AActor* Hostile);

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void HandleSensedHearing(AActor* Hostile);

	UPROPERTY(BlueprintCallable,BlueprintAssignable, Category = "ShooterProject")
	FOnHostileDetected OnHostileDetected;

private:
	void HandleSense(AActor* Hostile);
};
