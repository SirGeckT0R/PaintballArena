// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterProjectGameModeBase.h"

void AShooterProjectGameModeBase::BeginPlay() {
	Super::BeginPlay();
}

APatrolRoute* AShooterProjectGameModeBase::GetNearestPatrolRouteToActor(AActor* Actor)
{
	float MinDistance = -1;
	APatrolRoute* NearestPatrolRoute = nullptr;
	if (!PatrolRoutes.IsEmpty()) {
		for (int i = 0; i < PatrolRoutes.Num(); i++) {
			float TempDistance = Actor->GetDistanceTo(PatrolRoutes[i]);
			MinDistance = MinDistance < 0 ? TempDistance : MinDistance;

			if (!PatrolRoutes[i]->IsTaken && (TempDistance <= MinDistance)) {
				MinDistance = TempDistance;
				NearestPatrolRoute = PatrolRoutes[i];
			}
		}

		if (NearestPatrolRoute != nullptr) {
			return NearestPatrolRoute;
		}
		else {
			return PatrolRoutes[FMath::RandRange(0, PatrolRoutes.Num())];
		}
	}
	
	return nullptr;
}


