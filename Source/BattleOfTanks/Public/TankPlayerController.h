// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEOFTANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATankPlayerController();

	virtual void Tick(float DeltaTime) override;

	ATank* GetControlledTank() const { return Cast<ATank>(GetPawn()); }

protected:
	virtual void BeginPlay() override;

private:
	//��ȡ׼���������߻��е�λ��, ���Ϊ��, �����
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	//������̨��׼׼�ǵ�ָ��
	void AimTowardsCrosshair();

	//׼����Ļ�������
	UPROPERTY(EditDefaultsOnly, Category = "Cross Hair")
	FVector2D CrossHairLocationScale;



};
