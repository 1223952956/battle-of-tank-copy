// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpItem.h"
#include "PickUpProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEOFTANKS_API APickUpProjectile : public APickUpItem
{
	GENERATED_BODY()

public:
	APickUpProjectile();

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	int32 Amount;

	UPROPERTY(EditDefaultsOnly, Category = "Identify")
	int32 TypeNum;

};
