// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrelStaticMeshComponent.h"


UTankBarrelStaticMeshComponent::UTankBarrelStaticMeshComponent() {
	MaxDegreesPerSecond = 5.0f;
	MaxElevationDegree = 40.0f;
	MinElevationdegree = 0.0f;
}

void UTankBarrelStaticMeshComponent::Elevate(float RelativeSpeed) {
	//UE_LOG(LogTemp, Warning, TEXT("Elevate speed: %f"), RelativeSpeed);
	//获取每帧旋转角度
	auto DeltaElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto NewElevateRotation = GetRelativeRotation();
	NewElevateRotation.Pitch += DeltaElevationChange;
	//限制旋转角度
	NewElevateRotation.Pitch = FMath::Clamp(NewElevateRotation.Pitch, MinElevationdegree, MaxElevationDegree);
	//设置旋转角度
	SetRelativeRotation(NewElevateRotation);
}