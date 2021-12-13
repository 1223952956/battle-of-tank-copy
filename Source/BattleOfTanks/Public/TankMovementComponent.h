// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrackStaticMeshComponent;

/**
 * 
 */
UCLASS()
class BATTLEOFTANKS_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()


public:
	void IntendMoveForward(float Throw);

private:
	UTankTrackStaticMeshComponent* LeftTrack;
	UTankTrackStaticMeshComponent* RightTrack;
};
