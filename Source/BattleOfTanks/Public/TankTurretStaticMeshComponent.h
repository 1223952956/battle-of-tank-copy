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

	//��ת����(��̨)
	void Rotate(float RelativeSpeed);

	UFUNCTION(Server, unreliable)
	void SetRotationServer(FRotator NewRotation);

	UFUNCTION(NetMulticast, unreliable)
	void SetRotationClient(FRotator NewRotation);


private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDegreesPerSecond;

	//UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//float MaxRotationDegree;

	//UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//float MinRotationdegree;
	
};
