// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERPROJECT_API AAIEnemyBase : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	class AActor* AttackTarget;

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

	//virtual void OnPossess(APawn* PossessedPawn) override;
};
