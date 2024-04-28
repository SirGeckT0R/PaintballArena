// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PatrolRoute.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERPROJECT_API AShooterProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	TArray<APatrolRoute*> PatrolRoutes;

	UFUNCTION(BlueprintCallable)
		void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	APatrolRoute* GetNearestPatrolRouteToActor(AActor* Actor);
};

