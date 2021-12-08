// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrelStaticMeshComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UTankAimingComponent::AimAt(FVector WorldSpaceAim, float LaunchSpeed) {	
	//��ȡ�ڹܿ���λ��
	if (!Barrel) return;
	FVector FireLocation;
	FRotator FireRotation;
	Barrel->GetSocketWorldLocationAndRotation(TEXT("Projectile"), FireLocation, FireRotation);
	//UE_LOG(LogTemp, Warning, TEXT("%s's FireRotation: %s"), *GetOwner()->GetName() , *FireRotation.ToString());

	//��ȡԤ���ٶ�
	FVector LaunchVelocity;
	if (!UGameplayStatics::SuggestProjectileVelocity(
		this,
		LaunchVelocity,
		FireLocation,
		WorldSpaceAim,
		LaunchSpeed,false,
		0.0f,
		0.0f,
		ESuggestProjVelocityTraceOption::DoNotTrace
		)) return;

	//��ȡԤ���ٶȱ�׼������
	auto AimDirection = LaunchVelocity.GetSafeNormal();


	//�ƶ��ڹ�
	MoveBarrelTowards(AimDirection);
	//��־���
	//UE_LOG(LogTemp, Warning, TEXT("Aiming at %s"), *AimDirection.ToString());
}

void UTankAimingComponent::SetBarrelReference(UTankBarrelStaticMeshComponent* BarrelToSet) {
	Barrel = BarrelToSet;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	//��directionת��Ϊrotation
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimRotation = AimDirection.Rotation();

	//����barrel�෽��ת��
	auto DeltaRotation = AimRotation - BarrelRotation;
	UE_LOG(LogTemp, Warning, TEXT("rotate from %s to %s"), *BarrelRotation.ToString(), *AimRotation.ToString());

	
	Barrel->Elevate(1.0f);
}

