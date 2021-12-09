// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrelStaticMeshComponent;
class UTankTurretStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLEOFTANKS_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	void SetBarrelAndTurretReference(UTankBarrelStaticMeshComponent* BarrelToSet, UTankTurretStaticMeshComponent* TurretToSet);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void MoveBarrelTowards(FVector AimDirection);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//瞄准世界某处
	void AimAt(FVector WorldSpaceAim, float LaunchSpeed);

private:
	UTankBarrelStaticMeshComponent* Barrel;
	UTankTurretStaticMeshComponent* Turret;
};
