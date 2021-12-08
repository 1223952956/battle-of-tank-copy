// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Tank.h"

void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	auto PlayerTank = GetPlayerTank();
	if (PlayerTank) {
		UE_LOG(LogTemp, Warning, TEXT("AI find player tank %s"), *(PlayerTank->GetName()));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AI not find player tank"));
	}
	
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	AimTowardsPlayer();
}

ATank* ATankAIController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const {
	auto World = GetWorld();
	if (World) {
		auto PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController != NULL) {
			auto PlayerTank = PlayerController->GetPawn();
			return Cast<ATank>(PlayerTank);
		}
	}
	return nullptr;

}

bool ATankAIController::GetPlayerTankLocation(FVector& OutLocation) const {
	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank) return false;

	OutLocation = PlayerTank->GetActorLocation();
	return true;
}

void ATankAIController::AimTowardsPlayer() {
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) return;

	FVector PlayerLocation;
	if (GetPlayerTankLocation(PlayerLocation)) {
		ControlledTank->AimAt(PlayerLocation);
	}
}