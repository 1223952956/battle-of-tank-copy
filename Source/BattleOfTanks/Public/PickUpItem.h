// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;

UCLASS()
class BATTLEOFTANKS_API APickUpItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	USphereComponent* CollisionComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Component")
	UParticleSystemComponent* IdleParticlesComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	UParticleSystem* OverlapParticles;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY(EditDefaultsOnly, Category = "Identify")
	int32 TypeNum;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	bool bRotate;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float RotationRate;
};
