// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAI.h"
#include "PatrolRoute.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyBase.generated.h"

UCLASS()
class SHOOTERPROJECT_API AEnemyBase : public ACharacter, public IEnemyAI
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	class AWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject")
	class APatrolRoute* PatrolRoute;

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void SpawnWeapon(FTransform SpawnTransform);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float SetMovementSpeed(E_MovementSpeed Speed); virtual float SetMovementSpeed_Implementation(E_MovementSpeed Speed) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetIdealRange(float& AttackRadius, float& DefendRadius); virtual void GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EquipWeapon();  virtual void EquipWeapon_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnequipWeapon(); virtual void UnequipWeapon_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Attack(UAnimMontage* MontageToPlay, bool UseDefaultAttackMontage); virtual void Attack_Implementation(UAnimMontage* MontageToPlay, bool UseDefaultAttackMontage) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnterCrouchedState(); virtual void EnterCrouchedState_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExitCrouchedState(); virtual void ExitCrouchedState_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	APatrolRoute* GetPatrolRoute(); virtual APatrolRoute* GetPatrolRoute_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ReleasePatrolRoute(); virtual bool ReleasePatrolRoute_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FindAndSetPatrolRoute(); virtual void FindAndSetPatrolRoute_Implementation() override;
};
