// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "AmmoManager.h"
#include "Projectile.h"

// Sets default values
ARifle::ARifle(const FObjectInitializer& ObjectInitializer)
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
void ARifle::BeginPlay()
{	
	Super::BeginPlay();
	
}

// Called every frame
void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARifle::Attack() {
	if (ProjectileClass != nullptr && AmmoManager->LeftInMagazine > 0 && !isReloading) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleComponent->GetComponentTransform(), SpawnInfo);
		if (Projectile != nullptr) {
			Projectile->SetVelocity(MuzzleComponent->GetForwardVector());
			AttackEffects(FHitResult());
			AmmoManager->DecreaseAmmo();
		}
	}
	else if(ProjectileClass != nullptr && !isReloading){
		StartReload();
	}
}

void ARifle::StartReload() {
	isReloading = true;
	ReloadEffects();
}

void ARifle::StopReload() {
	if (AmmoManager->Reload()) {
		isReloading = false;
	}
}