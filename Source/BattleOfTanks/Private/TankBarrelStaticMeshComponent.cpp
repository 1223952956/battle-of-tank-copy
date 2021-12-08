// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrelStaticMeshComponent.h"


UTankBarrelStaticMeshComponent::UTankBarrelStaticMeshComponent() {
	MaxDegreesPerSecond = 5.0f;
	MaxElevationDegree = 40.0f;
	MinElevationdegree = 0.0f;
}

void UTankBarrelStaticMeshComponent::Elevate(float RelativeSpeed) {
	//UE_LOG(LogTemp, Warning, TEXT("Elevate speed: %f"), RelativeSpeed);
	//��ȡÿ֡��ת�Ƕ�
	auto DeltaElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto NewElevateRotation = GetRelativeRotation();
	NewElevateRotation.Pitch += DeltaElevationChange;
	//������ת�Ƕ�
	NewElevateRotation.Pitch = FMath::Clamp(NewElevateRotation.Pitch, MinElevationdegree, MaxElevationDegree);
	//������ת�Ƕ�
	SetRelativeRotation(NewElevateRotation);
}