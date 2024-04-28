// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERPROJECT_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()
	
private:
	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	virtual bool MeleeAttack(FHitResult& OutHit);
};
