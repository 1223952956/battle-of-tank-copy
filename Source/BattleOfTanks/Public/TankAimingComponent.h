// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrelStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLEOFTANKS_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	void SetBarrelReference(UTankBarrelStaticMeshComponent* BarrelToSet);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void MoveBarrelTowards(FVector AimDirection);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//��׼����ĳ��
	void AimAt(FVector WorldSpaceAim, float LaunchSpeed);

private:
	UTankBarrelStaticMeshComponent* Barrel;

};
