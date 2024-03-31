// Fill out your copyright notice in the Description page of Project Settings.


#include "HitscanWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"


void AHitscanWeapon::Fire() {
	if (AmmoManager->LeftInMagazine > 0 && !isReloading) {
		FTransform MuzzleTransform = MuzzleComponent->GetComponentTransform();
		FVector TraceStart = MuzzleTransform.GetLocation();
		FVector TraceEnd = TraceStart + MuzzleTransform.GetUnitAxis(EAxis::X) * TraceDistance;
		FCollisionQueryParams QueryParams(TEXT("WeaponTrace"), false, this);
		AmmoManager->DecreaseAmmo();

		FHitResult Hit;
		FLinearColor Color;
		DrawDebugDirectionalArrow(GetWorld(), TraceStart, TraceEnd, 1000.0f, Color.ToFColor(true).Red, false, 1.0f, 0, 5.0f);
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn, QueryParams)) {
			if (Hit.GetActor() != nullptr) {
				FVector ShotFromDirection = (TraceEnd - TraceStart).GetSafeNormal();
				FPointDamageEvent DamageEvent(DamageAmount, Hit, ShotFromDirection, UDamageType::StaticClass());
				UGameplayStatics::ApplyDamage(Hit.GetActor(), DamageAmount, GetInstigatorController(), GetOwner(), nullptr);
				//Hit.GetActor()->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);
				
				FireEffects(Hit.Location, Hit.Normal);
			}
		}
	}else if(!isReloading) {
		StartReload();
		return;
	}

	
}
