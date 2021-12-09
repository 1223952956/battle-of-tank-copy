// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurretStaticMeshComponent.h"

UTankTurretStaticMeshComponent::UTankTurretStaticMeshComponent() {
	SetIsReplicated(true);

	MaxDegreesPerSecond = 60.0f;
	//MaxRotationDegree = 180.0f;
	//MinRotationdegree = -180.0f;
}

void UTankTurretStaticMeshComponent::Rotate(float RelativeSpeed) {
	//UE_LOG(LogTemp, Warning, TEXT("Rotate speed: %f"), MaxDegreesPerSecond)

	//限制相对速度
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.0f, 1.0f);

	//获取每帧旋转角度
	auto World = GetWorld();
	check(World != nullptr);
	auto DeltaRotation = RelativeSpeed * MaxDegreesPerSecond * World->DeltaTimeSeconds;

	//每帧旋转结果Rotation
	auto NewRotation = GetRelativeRotation();
	NewRotation.Yaw += DeltaRotation;

	//限制旋转角度
	//NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw, MinRotationdegree, MaxRotationDegree);

	//设置旋转角度
	SetRelativeRotation(NewRotation);
	SetRotationServer(NewRotation);
}

void UTankTurretStaticMeshComponent::SetRotationServer_Implementation(FRotator NewRotation) {
	SetRotationClient(NewRotation);
	SetRelativeRotation(NewRotation);
}

void UTankTurretStaticMeshComponent::SetRotationClient_Implementation(FRotator NewRotation) {
	auto Owner = GetOwner();
	check(Owner != nullptr);
	if (Owner->GetLocalRole() < ROLE_AutonomousProxy) {
		SetRelativeRotation(NewRotation);
	}
}