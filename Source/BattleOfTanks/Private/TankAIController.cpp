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
	
	//AimTowardsPlayer();
	//Fire();
	//RotateTowards();
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

void ATankAIController::AimTowardsPlayer(FVector Location) {
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) return;

	ControlledTank->AimAt(Location);
}

void ATankAIController::Fire() {
	ATank* ControlledTank = GetControlledTank();
	ControlledTank->StartFire();
}

void ATankAIController::RotateTowards(FVector Target) {

	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank) return;
	
	FVector MyForwardVector = ControlledTank->GetActorForwardVector();

	FVector TargetVector = Target - ControlledTank->GetActorLocation();
	FVector CrossResult = FVector::CrossProduct(MyForwardVector, TargetVector).GetSafeNormal();

	UE_LOG(LogTemp, Warning, TEXT("CrossResult : %s"), *CrossResult.ToString());

	float relativeScale = CrossResult.Z;
	
	ControlledTank->TurnRight(relativeScale);

}

void ATankAIController::StopRotate() {
	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank) return;

	ControlledTank->TurnRight(0.0f);
}

void ATankAIController::MoveForward() {
	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank) return;

	ControlledTank->MoveForward(1.0f);
}

void ATankAIController::StopMoveForward() {
	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank) return;

	ControlledTank->MoveForward(0.0f);
}


void ATankAIController::SwitchCannon() {
	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank) return;

	float randNum = FMath::RandRange(-1.0f, 1.0f);

	if (randNum > 0) {
		ControlledTank->StartSwitch(1);
	}
	else {
		ControlledTank->StartSwitch(-1);
	}

}