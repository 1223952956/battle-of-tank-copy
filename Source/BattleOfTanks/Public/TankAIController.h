// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;

/**
 * 
 */
UCLASS()
class BATTLEOFTANKS_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:

	ATank* GetControlledTank() const;

	void GetPlayerTank();

	bool GetPlayerTankLocation(FVector& OutLocation);

	//��׼���
	void AimTowardsPlayer();

	void Fire();

	ATank* PlayerTank;
};
