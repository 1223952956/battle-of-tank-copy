// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"

ATankPlayerController::ATankPlayerController() {
	PrimaryActorTick.bCanEverTick = true;

	CrossHairLocationScale.X = 0.5f;
	CrossHairLocationScale.Y = 0.3333f;
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

void ATankPlayerController::AimTowardsCrosshair() {
	if (!GetControlledTank()) return;

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("Look Location: %s"), *HitLocation.ToString());
	}
	
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const {
	int32 ViewportSizeX, ViewportSizeY;

	GetViewportSize(ViewportSizeX, ViewportSizeY);

	//准星屏幕坐标
	FVector2D ScreenPosition = FVector2D(ViewportSizeX * CrossHairLocationScale.X, ViewportSizeY * CrossHairLocationScale.Y);
	
	FVector WorldLocation;
	FVector WorldDirection;
	
	//屏幕坐标转世界坐标
	if (!DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection)) return false;

	auto World = GetWorld();
	check(World != nullptr);

	//UE_LOG(LogTemp, Warning, TEXT("World Location: %s"), *WorldLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("World Direction: %s"), *WorldDirection.ToString());

	//射线检测
	FHitResult HitResult;
	
	DrawDebugLine(GetWorld(), WorldLocation, WorldLocation + 100000.0f * WorldDirection, FColor::Red);
	if (World->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + 100000.0f * WorldDirection, ECollisionChannel::ECC_WorldStatic))
	{
		OutHitLocation = HitResult.ImpactPoint;
		return true;
	}
	return false;

	

	
}