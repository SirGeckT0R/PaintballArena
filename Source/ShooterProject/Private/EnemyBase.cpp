// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "../ShooterProjectGameModeBase.h"
#include "Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponClass = AWeapon::StaticClass();
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::SpawnWeapon(FTransform SpawnTransform) {
	if (WeaponClass != nullptr) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = this;
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnTransform, SpawnInfo);
	}
}

float AEnemyBase::SetMovementSpeed_Implementation(E_MovementSpeed Speed)
{
	float MovementSpeedValue = 0;
	switch (Speed) {
	case E_MovementSpeed::Idle:
		MovementSpeedValue = 0;
		break;
	case E_MovementSpeed::Walking:
		MovementSpeedValue = 200;
		break;
	case E_MovementSpeed::Jogging:
		MovementSpeedValue = 300;
		break;
	case E_MovementSpeed::Running:
		MovementSpeedValue = 600;
		break;
	default:
		MovementSpeedValue = 0;
	}
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeedValue;
	return MovementSpeedValue;
}

void AEnemyBase::EnterCrouchedState_Implementation()
{
	this->Crouch();
}

void AEnemyBase::ExitCrouchedState_Implementation()
{
	this->UnCrouch();
}

APatrolRoute* AEnemyBase::GetPatrolRoute_Implementation()
{
	return PatrolRoute;
}

bool AEnemyBase::ReleasePatrolRoute_Implementation()
{
	if (PatrolRoute) {
		PatrolRoute->IsTaken = false;
	}

	return true;

}

void AEnemyBase::FindAndSetPatrolRoute_Implementation()
{
	AShooterProjectGameModeBase* GameMode = Cast<AShooterProjectGameModeBase>(UGameplayStatics::GetGameMode(this));	
	if (GameMode) {
		APatrolRoute* NearestPatrolRoute = GameMode->GetNearestPatrolRouteToActor(this);
		if (NearestPatrolRoute) {
			PatrolRoute = NearestPatrolRoute;
			PatrolRoute->IsTaken = true;
		}
	}
}

void AEnemyBase::GetIdealRange_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = 150;
	DefendRadius = 550;
}

void AEnemyBase::EquipWeapon_Implementation()
{
}

void AEnemyBase::UnequipWeapon_Implementation()
{
}

void AEnemyBase::Attack_Implementation(UAnimMontage* MontageToPlay, bool UseDefaultAttackMontage)
{

}
