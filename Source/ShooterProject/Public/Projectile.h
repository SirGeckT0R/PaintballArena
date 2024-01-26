// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Components/ShapeComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class SHOOTERPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere,Category = "ShooterProject")
	float DamageValue = 0;

	UPROPERTY(EditAnywhere, Category = "ShooterProject")
	float InitialSpeed = 0;

	UPROPERTY(EditAnywhere, Category = "ShooterProject")
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "ShooterProject")
	class UShapeComponent* CollisionComponent;

	// Sets default values for this actor's properties
	AProjectile(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void DoDamage(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void SetVelocity(FVector Direction);
};
