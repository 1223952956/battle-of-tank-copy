// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrackStaticMeshComponent.generated.h"

/**
 *坦克履带被用于设定最大驱动速度, 并应用与坦克
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class BATTLEOFTANKS_API UTankTrackStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankTrackStaticMeshComponent();

	//设置油门大小 -1 到 +1
	UFUNCTION()
	void SetThrottle(float Throttle);

private:
	//每个履带的最大力/N
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce;

	UFUNCTION(Server, unreliable)
	void AddForceServer(float Throttle);

};
