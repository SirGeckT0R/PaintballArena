// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileMovement"));
	CollisionComponent = ObjectInitializer.CreateDefaultSubobject<UShapeComponent>(this, TEXT("CollisionComponent"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);

	if (MovementComponent != nullptr) {
		MovementComponent->InitialSpeed = InitialSpeed;
		MovementComponent->InitialSpeed = InitialSpeed;
		MovementComponent->bShouldBounce = true;
		MovementComponent->Bounciness = 0.3f;
		MovementComponent->ProjectileGravityScale = 0.0f;
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::SetVelocity(FVector Direction) {
	if (MovementComponent != nullptr) {
		MovementComponent->Velocity = Direction * InitialSpeed;
	}
}

void AProjectile::DoDamage(AActor* OtherActor) {
	if (OtherActor != nullptr) {
		UGameplayStatics::ApplyDamage(OtherActor, DamageValue, nullptr, nullptr, nullptr);
		Destroy();
	}

}
