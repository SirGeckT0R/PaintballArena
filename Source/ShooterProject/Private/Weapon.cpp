// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "AmmoManager.h"
#include "Projectile.h"

// Sets default values
AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));
	MuzzleComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("MuzzleComponent"));
	MuzzleComponent->SetupAttachment(RootComponent);

	ProjectileClass = AProjectile::StaticClass();

	AmmoManager = ObjectInitializer.CreateDefaultSubobject<UAmmoManager>(this, TEXT("AmmoManager"));
	AddOwnedComponent(AmmoManager);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{	
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire() {
	if (ProjectileClass != nullptr && AmmoManager->LeftInMagazine > 0 && !isReloading) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleComponent->GetComponentTransform(), SpawnInfo);
		if (Projectile != nullptr) {
			Projectile->SetVelocity(MuzzleComponent->GetForwardVector());
			FireEffects(FVector(),FVector());
			AmmoManager->DecreaseAmmo();
		}
	}
	else if(ProjectileClass != nullptr && !isReloading){
		StartReload();
	}
}

void AWeapon::StartReload() {
	isReloading = true;
	ReloadEffects();
}

void AWeapon::StopReload() {
	if (AmmoManager->Reload()) {
		isReloading = false;
	}
}