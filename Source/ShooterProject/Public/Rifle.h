// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoManager.h"
#include "Weapon.h"
#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

UCLASS(BlueprintType)
class SHOOTERPROJECT_API ARifle : public AWeapon
{
	GENERATED_BODY()

protected:
	bool isReloading=false;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	class USceneComponent* MuzzleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	UAmmoManager* AmmoManager;

	// Sets default values for this actor's properties
	ARifle(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void StartReload();

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void StopReload();
		
	UFUNCTION(BlueprintImplementableEvent, Category = "ShooterProject")
	void ReloadEffects();
};
