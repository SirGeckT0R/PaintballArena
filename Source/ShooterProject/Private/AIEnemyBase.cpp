// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyBase.h"
#include "EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void AAIEnemyBase::OnPossess(APawn* PossessedPawn)
{
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
		}
	}

}

E_State AAIEnemyBase::GetCurrentState()
{
	return E_State(BlackboardBase->GetValueAsEnum(StateKeyName));
}

void AAIEnemyBase::SetStateAsPassive()
{
	BlackboardBase->SetValueAsEnum(StateKeyName, E_State::Passive);
}

void AAIEnemyBase::SetStateAsAttacking(AActor* Target, bool UsePreviousKnownAttackTarget)
{
	E_State CurrentState = E_State(BlackboardBase->GetValueAsEnum(StateKeyName));
	AActor* NewAttackTarget;
	if (this->AttackTarget != nullptr && UsePreviousKnownAttackTarget) {
		if (CurrentState != E_State::Dead) {
			NewAttackTarget = this->AttackTarget;
		}
	}
	else {
		if (AttackTarget != nullptr) {

			NewAttackTarget = Target;
			this->AttackTarget = Target;
			BlackboardBase->SetValueAsEnum(StateKeyName, E_State::Attacking);
			BlackboardBase->SetValueAsObject(AttackTargetKeyName, NewAttackTarget);
		}
		else {
			SetStateAsPassive();
		}
	}
}

void AAIEnemyBase::SetStateAsInvestigating(FVector Location)
{
	BlackboardBase->SetValueAsEnum(StateKeyName, E_State::Investigating);
	BlackboardBase->SetValueAsVector(PointOfInterestKeyName, Location);
}

void AAIEnemyBase::SetStateAsFrozen()
{
	BlackboardBase->SetValueAsEnum(StateKeyName, E_State::Frozen);
}

void AAIEnemyBase::SetStateAsDead()
{
	BlackboardBase->SetValueAsEnum(StateKeyName, E_State::Dead);
}

void AAIEnemyBase::SetStateAsPatrolling()
{
	BlackboardBase->SetValueAsEnum(StateKeyName, E_State::Patrolling);
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
	E_State CurrentState = GetCurrentState();
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	switch (CurrentState) {
	case E_State::Passive:
	case E_State::Patrolling:
	case E_State::Investigating:
		if (PlayerCharacter == Hostile) {
			SetStateAsAttacking(Hostile, false);
			OnHostileDetected.Execute(Hostile);
		}
		break;
	default:
		break;
	}
}
