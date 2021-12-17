// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

class ATank;

UCLASS()
class BATTLEOFTANKS_API AShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShield();

	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AttachToTank(ATank* InOwnerTank, int32 InStorageIndex);

	void UnAttach();

	//UFUNCTION(Server, Reliable)
	void Equip(int32 InSlotIndex);

	//UFUNCTION(Server, Reliable)
	void UnEquip();


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Component")
	UStaticMeshComponent* TestMeshComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float MaxLifeTime;

	float CurrentLifeTime;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DefenceValue;

	bool bIsEquipped;

	//UPROPERTY(Replicated)
	ATank* OwnerTank;

	int32 SlotIndex;

	int32 StorageIndex;
};
