// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTurretStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEOFTANKS_API UTankTurretStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankTurretStaticMeshComponent();

	//旋转自身(炮台)
	void Rotate(float RelativeSpeed);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDegreesPerSecond;

	UFUNCTION(Server, unreliable)
	void SetRotationServer(FRotator NewRotation);

	UFUNCTION(NetMulticast, unreliable)
	void SetRotationClient(FRotator NewRotation);

	//UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//float MaxRotationDegree;

	//UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//float MinRotationdegree;
	
};
