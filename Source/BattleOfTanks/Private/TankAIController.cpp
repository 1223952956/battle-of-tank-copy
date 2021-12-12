// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Tank.h"

void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	//auto PlayerTank = GetPlayerTank();
	//if (PlayerTank) {
	//	UE_LOG(LogTemp, Warning, TEXT("AI find player tank %s"), *(PlayerTank->GetName()));
	//}
	//else {
	//	UE_LOG(LogTemp, Warning, TEXT("AI not find player tank"));
	//}
	
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	AimTowardsPlayer();
	Fire();
}

ATank* ATankAIController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

void ATankAIController::GetPlayerTank() {
	auto World = GetWorld();
	check(World != nullptr);

	auto PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController != NULL) {
		PlayerTank = Cast<ATank>(PlayerController->GetPawn());
	}


}

bool ATankAIController::GetPlayerTankLocation(FVector& OutLocation) {
	GetPlayerTank();
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

void ATankAIController::Fire() {
	ATank* ControlledTank = GetControlledTank();
	ControlledTank->StartFire();
}