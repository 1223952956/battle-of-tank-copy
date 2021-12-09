// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrelStaticMeshComponent.h"
#include "TankTurretStaticMeshComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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
	//获取炮管开火位置
	if (!Barrel) return;
	FVector FireLocation;
	FRotator FireRotation;
	Barrel->GetSocketWorldLocationAndRotation(TEXT("Projectile"), FireLocation, FireRotation);
	//UE_LOG(LogTemp, Warning, TEXT("%s's FireRotation: %s"), *GetOwner()->GetName() , *FireRotation.ToString());

	//获取预测速度
	FVector LaunchVelocity;
	if (!UGameplayStatics::SuggestProjectileVelocity(
		this,
		LaunchVelocity,
		FireLocation,
		WorldSpaceAim,
		LaunchSpeed,
		false,
		0.0f,
		0.0f,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam,
		TArray<AActor*>(),
		false
		)) return;

	//获取预测速度标准化向量
	auto AimDirection = LaunchVelocity.GetSafeNormal();


	//移动炮管
	MoveBarrelTowards(AimDirection);
	//日志输出
	//UE_LOG(LogTemp, Warning, TEXT("Aiming at %s"), *AimDirection.ToString());
}

void UTankAimingComponent::SetBarrelAndTurretReference(UTankBarrelStaticMeshComponent* BarrelToSet, UTankTurretStaticMeshComponent* TurretToSet) {
	check(BarrelToSet != nullptr && TurretToSet != nullptr);
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	//将direction转换为rotation
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimRotation = AimDirection.Rotation();

	//获得转动差
	auto DeltaRotation = AimRotation - BarrelRotation;
	//UE_LOG(LogTemp, Warning, TEXT("rotate from %s to %s"), *BarrelRotation.ToString(), *AimRotation.ToString());

	if (FMath::Abs(DeltaRotation.Yaw) >= 180.0f)
	{
		DeltaRotation.Yaw = DeltaRotation.Yaw > 0 ? DeltaRotation.Yaw - 360.0f : DeltaRotation.Yaw + 360.0f;
	}

	//调用炮管和炮台旋转
	Barrel->Elevate(DeltaRotation.Pitch);
	Turret->Rotate(DeltaRotation.Yaw);
}

