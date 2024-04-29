// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damagable.h"
#include "Components/ActorComponent.h"
#include "DamageSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlocked, bool, CanBeParried);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageResponse, EDamageResponse, DamageResponse, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERPROJECT_API UDamageSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageSystemComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	bool IsInvincible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	bool IsInterruptible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	bool IsBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	float MaxHealth;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	float Heal(float Amount);

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	bool TakeDamage(FSDamageInfo DamageInfo, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	bool BlockDamage(bool CanBeParried);

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	bool DoDamage(float Amount, EDamageResponse DamageResponse, bool ShouldForceInterrupt, AActor* Causer);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "ShooterProject")
	FOnBlocked OnBlocked;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "ShooterProject")
	FOnDeath OnDeath;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "ShooterProject")
	FOnDamageResponse OnDamageResponse;
};
