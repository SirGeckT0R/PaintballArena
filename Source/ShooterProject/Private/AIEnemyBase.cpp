// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyBase.h"
#include "EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void AAIEnemyBase::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
	AEnemyBase* BaseEnemy = Cast<AEnemyBase>(PossessedPawn);
	if (BaseEnemy != nullptr) {
		if (BaseEnemy->BehaviorTree !=nullptr) {
			this->RunBehaviorTree(BaseEnemy->BehaviorTree);
			float AttackRadius = 0;
			float DefendRadius = 0;
			BaseEnemy->GetIdealRange(AttackRadius, DefendRadius);
			UseBlackboard(BlackboardToUse, BlackboardBase);
			BlackboardBase->SetValueAsFloat(AttackRadiusKeyName, AttackRadius);
			BlackboardBase->SetValueAsFloat(DefendRadiusKeyName, DefendRadius);
			BaseEnemy->FindAndSetPatrolRoute();
			SetStateAsPatrolling();
		}
	}

}

EState AAIEnemyBase::GetCurrentState()
{
	return EState(BlackboardBase->GetValueAsEnum(StateKeyName));
}

void AAIEnemyBase::SetStateAsPassive()
{
	BlackboardBase->SetValueAsEnum(StateKeyName, EState::Passive);
}

void AAIEnemyBase::SetStateAsAttacking(AActor* Target, bool UsePreviousKnownAttackTarget)
{
	EState CurrentState = EState(BlackboardBase->GetValueAsEnum(StateKeyName));
	AActor* NewAttackTarget;
	if (this->AttackTarget != nullptr && UsePreviousKnownAttackTarget) {
		if (CurrentState != EState::Dead) {
			NewAttackTarget = this->AttackTarget;
		}
	}
	else {
		if (Target != nullptr) {

			NewAttackTarget = Target;
			this->AttackTarget = Target;
			BlackboardBase->SetValueAsEnum(StateKeyName, EState::Attacking);
			BlackboardBase->SetValueAsObject(AttackTargetKeyName, NewAttackTarget);
		}
		else {
			SetStateAsPassive();
		}
	}
}

void AAIEnemyBase::SetStateAsInvestigating(FVector Location)
{
	BlackboardBase->SetValueAsEnum(StateKeyName, EState::Investigating);
	BlackboardBase->SetValueAsVector(PointOfInterestKeyName, Location);
}

void AAIEnemyBase::SetStateAsFrozen()
{
	BlackboardBase->SetValueAsEnum(StateKeyName, EState::Frozen);
}

void AAIEnemyBase::SetStateAsDead()
{
	BlackboardBase->SetValueAsEnum(StateKeyName, EState::Dead);
}

void AAIEnemyBase::SetStateAsPatrolling()
{
	BlackboardBase->SetValueAsEnum(StateKeyName, EState::Patrolling);
}

void AAIEnemyBase::SetSpawnPointLocation(FVector Location)
{
	BlackboardBase->SetValueAsVector(SpawnPointKeyName, Location);
}

void AAIEnemyBase::HandleSensedSight(AActor* Hostile)
{
	HandleSense(Hostile);
}

void AAIEnemyBase::HandleSensedDamage(AActor* Hostile)
{
	HandleSense(Hostile);
}

void AAIEnemyBase::HandleSensedHearing(AActor* Hostile)
{
	HandleSense(Hostile);
}

void AAIEnemyBase::HandleSense(AActor* Hostile)
{
	EState CurrentState = GetCurrentState();
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	switch (CurrentState) {
	case EState::Passive:
	case EState::Patrolling:
	case EState::Investigating:
		if (PlayerCharacter == Hostile) {
			SetStateAsAttacking(Hostile, false);
			OnHostileDetected.Broadcast(Hostile);
		}
		break;
	default:
		break;
	}
}
