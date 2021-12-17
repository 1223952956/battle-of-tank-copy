// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpItem.h"
#include "PickUpShield.generated.h"

class AShield;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class BATTLEOFTANKS_API APickUpShield : public APickUpItem
{
	GENERATED_BODY()

public:
	APickUpShield();

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	//UFUNCTION(NetMulticast, Reliable)
	//void PickUpMulticast(FString InShieldName);
protected:
	//virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AShield> ShieldClass;

};
