// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rifle.h"
#include "HitscanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERPROJECT_API AHitscanWeapon : public ARifle
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "ShooterProject")
	float TraceDistance = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "ShooterProject")
	float DamageAmount = 15.0f;

	virtual void Attack() override;
};
