// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrelStaticMeshComponent.h"


UTankBarrelStaticMeshComponent::UTankBarrelStaticMeshComponent() {
	//SetIsReplicated(true);

	MaxDegreesPerSecond = 20.0f;
	MaxElevationDegree = 40.0f;
	MinElevationdegree = 0.0f;
}

void UTankBarrelStaticMeshComponent::Elevate(float RelativeSpeed) {
	//UE_LOG(LogTemp, Warning, TEXT("Elevate speed: %f"), RelativeSpeed);
	
	//限制相对速度
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.0f, 1.0f);

	//获取每帧旋转角度
	auto World = GetWorld();
	check(World != nullptr);
	auto DeltaElevation = RelativeSpeed * MaxDegreesPerSecond * World->DeltaTimeSeconds;

	//每帧旋转结果Rotation
	auto NewRotation = GetRelativeRotation();
	NewRotation.Pitch += DeltaElevation;
	
	//限制旋转角度
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, MinElevationdegree, MaxElevationDegree);
	
	//设置旋转角度
	SetRelativeRotation(NewRotation);
	SetElevationServer(NewRotation);
	
}

void UTankBarrelStaticMeshComponent::SetElevationServer_Implementation(FRotator NewElevation) {
	SetRelativeRotation(NewElevation);
	SetElevationClient(NewElevation);
}

void UTankBarrelStaticMeshComponent::SetElevationClient_Implementation(FRotator NewElevation) {
	auto Owner = GetOwner();
	check(Owner != nullptr);
	if (Owner->GetLocalRole() < ROLE_AutonomousProxy) {
		SetRelativeRotation(NewElevation);
	}
}
