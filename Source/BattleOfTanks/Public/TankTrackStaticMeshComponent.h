// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrackStaticMeshComponent.generated.h"

/**
 *̹���Ĵ��������趨��������ٶ�, ��Ӧ����̹��
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class BATTLEOFTANKS_API UTankTrackStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankTrackStaticMeshComponent();

	//�������Ŵ�С -1 �� +1
	UFUNCTION()
	void SetThrottle(float Throttle);

private:
	//ÿ���Ĵ��������/N
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce;

	UFUNCTION(Server, unreliable)
	void AddForceServer(float Throttle);

};
