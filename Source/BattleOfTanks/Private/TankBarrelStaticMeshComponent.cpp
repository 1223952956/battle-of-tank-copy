// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrelStaticMeshComponent.h"


UTankBarrelStaticMeshComponent::UTankBarrelStaticMeshComponent() {
	MaxDegreesPerSecond = 20.0f;
	MaxElevationDegree = 40.0f;
	MinElevationdegree = 0.0f;
}

void UTankBarrelStaticMeshComponent::Elevate(float RelativeSpeed) {
	//UE_LOG(LogTemp, Warning, TEXT("Elevate speed: %f"), RelativeSpeed);
	
	//��������ٶ�
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.0f, 1.0f);

	//��ȡÿ֡��ת�Ƕ�
	auto World = GetWorld();
	check(World != nullptr);
	auto DeltaElevation = RelativeSpeed * MaxDegreesPerSecond * World->DeltaTimeSeconds;

	//ÿ֡��ת���Rotation
	auto NewRotation = GetRelativeRotation();
	NewRotation.Pitch += DeltaElevation;
	
	//������ת�Ƕ�
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, MinElevationdegree, MaxElevationDegree);
	
	//������ת�Ƕ�
	SetRelativeRotation(NewRotation);
}