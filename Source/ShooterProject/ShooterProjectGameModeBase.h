// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION(BlueprintCallable)
		void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		float HeyFunction();
};
