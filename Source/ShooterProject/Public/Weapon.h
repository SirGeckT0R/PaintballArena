// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoManager.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS(BlueprintType)
class SHOOTERPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	class USceneComponent* MuzzleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	UAmmoManager* AmmoManager;

	// Sets default values for this actor's properties
	AWeapon(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	virtual void Fire();

	UFUNCTION(BlueprintImplementableEvent, Category = "ShooterProject")
	void FireEffects();

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void Reload();
		
	UFUNCTION(BlueprintImplementableEvent, Category = "ShooterProject")
	void ReloadEffects();
};
