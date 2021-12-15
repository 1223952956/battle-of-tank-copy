// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Tank.h"
#include "DrawDebugHelpers.h"

ATankPlayerController::ATankPlayerController() {
	PrimaryActorTick.bCanEverTick = true;

	CrossHairLocationScale.X = 0.5f;
	CrossHairLocationScale.Y = 0.3333f;

	LineTraceRange = 50000.0f;
}


void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();
	auto ControlledTank = GetControlledTank();
	if (ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("PlayerController possesing a tank %s"), *(ControlledTank->GetName()));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not possesing a tank"));
	}
}

void ATankPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair() {
	ATank* ControlledTank = GetControlledTank();
	if (!ControlledTank) return;

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		ControlledTank->AimAt(HitLocation);
	}
	
}

//void ATankPlayerController::AimTowardsCrosshair_Implementation() {
//	ATank* ControlledTank = GetControlledTank();
//	if (!ControlledTank) return;
//
//	FVector HitLocation;
//	if (GetSightRayHitLocation(HitLocation))
//	{
//		ControlledTank->AimAt(HitLocation);
//	}
//}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const {
	int32 ViewportSizeX, ViewportSizeY;

	GetViewportSize(ViewportSizeX, ViewportSizeY);

	//准星屏幕坐标
	FVector2D ScreenPosition = FVector2D(ViewportSizeX * CrossHairLocationScale.X, ViewportSizeY * CrossHairLocationScale.Y);
	
	FVector WorldLocation;
	FVector WorldDirection;
	
	//屏幕坐标转世界坐标
	if (!DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection)) return false;

	UWorld* World = GetWorld();
	check(World != nullptr);

	//UE_LOG(LogTemp, Warning, TEXT("World Location: %s"), *WorldLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("World Direction: %s"), *WorldDirection.ToString());

	//射线检测
	FHitResult HitResult;
	FVector StartLocation = WorldLocation;
	FVector EndLocation = WorldLocation + LineTraceRange * WorldDirection;
	
	//DrawDebugLine(GetWorld(), WorldLocation, WorldLocation + LineTraceRange * WorldDirection, FColor::Red);
	if (World->LineTraceSingleByChannel(
		HitResult, 
		StartLocation, 
		EndLocation, 
		ECollisionChannel::ECC_Visibility))
	{
		OutHitLocation = HitResult.Location;
		//UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *OutHitLocation.ToString());
		return true;
	}
	OutHitLocation = WorldLocation + LineTraceRange / 10 * WorldDirection;
	return true;

	

	
}