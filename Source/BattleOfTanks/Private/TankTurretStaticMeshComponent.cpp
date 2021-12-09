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

	//��������ٶ�
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.0f, 1.0f);

	//��ȡÿ֡��ת�Ƕ�
	auto World = GetWorld();
	check(World != nullptr);
	auto DeltaRotation = RelativeSpeed * MaxDegreesPerSecond * World->DeltaTimeSeconds;

	//ÿ֡��ת���Rotation
	auto NewRotation = GetRelativeRotation();
	NewRotation.Yaw += DeltaRotation;

	//������ת�Ƕ�
	//NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw, MinRotationdegree, MaxRotationDegree);

	//������ת�Ƕ�
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