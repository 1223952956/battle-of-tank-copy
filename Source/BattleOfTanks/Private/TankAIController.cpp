// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"

void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	Target = GetPlayerTank();
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

	//FVector PlayerLocation;
	//if (GetPlayerTankLocation(PlayerLocation)) {
	//	UE_LOG(LogTemp, Warning, TEXT("Player tank Location: %s"), *PlayerLocation.ToString());
	//}
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
	if (!Target) return false;

	OutLocation = Target->GetActorLocation();
	return true;
}