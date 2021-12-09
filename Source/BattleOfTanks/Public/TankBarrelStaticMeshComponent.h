// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrelStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class BATTLEOFTANKS_API UTankBarrelStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UTankBarrelStaticMeshComponent();

	//Ì§Éý×ÔÉí(ÅÚ¹Ü)
	void Elevate(float RelativeSpeed);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDegreesPerSecond;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxElevationDegree;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MinElevationdegree;
};
