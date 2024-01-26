// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoManager.h"

// Sets default values for this component's properties
UAmmoManager::UAmmoManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UAmmoManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAmmoManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//UAmmoManager* UAmmoManager::CreateAmmoManager(int Max = 0, int MagCapacity = 0) {
//		UAmmoManager* manager = NewObject<UAmmoManager>();
//		manager->Initialize();
//		return manager;
//}
//
//void UAmmoManager::Initialize(int Max = 0, int MagCapacity = 0) {
//	MaxLoad = Max;
//	MagazineCapacity = MagCapacity;
//}

int UAmmoManager::GetMagazineCapacity() {
	return MagazineCapacity;
}

int UAmmoManager::GetMaxLoad() {
	return MaxLoad;
}

void UAmmoManager::DecreaseAmmo(int amount) {
	LeftInMagazine -= amount;
	TotalLeft -= amount;
}

bool UAmmoManager::Reload() {
	if (TotalLeft > 0) {
		int mag = GetMagazineCapacity();
		if (TotalLeft == LeftInMagazine || mag == LeftInMagazine) {
			return false;
		}

		if (TotalLeft >= mag) {
			LeftInMagazine = mag;
		}
		else {
			LeftInMagazine = TotalLeft;
		}
		return true;
	}

	return false;
}