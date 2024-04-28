// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Kismet/KismetSystemLibrary.h"

bool AMeleeWeapon::MeleeAttack(FHitResult& OutHit)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;	
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	FVector StartLocation = { 0,0,0 };
	FVector EndLocation = { 0,0,0 };
	TArray<AActor*, FDefaultAllocator> EmptyArray;
	bool bHasHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLocation, EndLocation, 40.0f, ObjectTypesArray, false, EmptyArray, EDrawDebugTrace::ForDuration, OutHit, true);
	return bHasHit;
}
