// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageSystemComponent.h"

// Sets default values for this component's properties
UDamageSystemComponent::UDamageSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDamageSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDamageSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UDamageSystemComponent::Heal(float Amount) {
	if (!IsDead) {
		CurrentHealth = (CurrentHealth + Amount) > MaxHealth ? MaxHealth : (CurrentHealth + Amount);
		return CurrentHealth;
	}
	return 0;
}

bool UDamageSystemComponent::TakeDamage(FSDamageInfo DamageInfo, AActor* DamageCauser) {
	if ((!IsInvincible || DamageInfo.ShouldDamageInvincible) && !IsDead) {
		if (IsBlocking && DamageInfo.CanBeBlocked) {
			return BlockDamage(DamageInfo.CanBeParried);
		}
		else {
			return DoDamage(DamageInfo.Amount, DamageInfo.DamageResponse, DamageInfo.ShouldForceInterrupt, DamageCauser);
		}
	}
	return false;
}

bool UDamageSystemComponent::BlockDamage(bool CanBeParried) {
	OnBlocked.Broadcast(CanBeParried);
	return false;
}

bool UDamageSystemComponent::DoDamage(float Amount, EDamageResponse DamageResponse, bool ShouldForceInterrupt, AActor* Causer) {
	CurrentHealth -= Amount;
	if (CurrentHealth <= 0) {
		IsDead = true;
		OnDeath.Broadcast();
	}
	else if(ShouldForceInterrupt || IsInterruptible){
		OnDamageResponse.Broadcast(DamageResponse, Causer);
	}

	return true;
}
