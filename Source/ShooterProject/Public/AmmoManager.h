// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERPROJECT_API UAmmoManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAmmoManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject", meta = (ClampMin = "0.0"))
	int LeftInMagazine = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShooterProject", meta = (ClampMin = "0.0"))
	int TotalLeft = 0;

	/*UFUNCTION()
	static UAmmoManager* CreateAmmoManager(int Max = 0, int MagCapacity = 0);

	UFUNCTION()
	void Initialize(int Max = 0, int MagCapacity = 0);*/

	UFUNCTION()
	int GetMagazineCapacity();

	UFUNCTION()
	int GetMaxLoad();

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	bool Reload();

	UFUNCTION(BlueprintCallable, Category = "ShooterProject")
	void DecreaseAmmo(int amount = 1);

private:
	UPROPERTY(EditDefaultsOnly, Category = "ShooterProject")
	int MaxLoad = 0;

	UPROPERTY(EditDefaultsOnly, Category = "ShooterProject")
	int MagazineCapacity = 0;
		
};
