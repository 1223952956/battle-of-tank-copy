// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATank;


UCLASS()
class BATTLEOFTANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATankPlayerController();

	virtual void Tick(float DeltaTime) override;

	
protected:
	virtual void BeginPlay() override;

private:
	ATank* GetControlledTank() const;

	//��ȡ׼���������߻��е�λ��, ���Ϊ��, �����
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	//������̨��׼׼�ǵ�ָ��
	//UFUNCTION(Server, unreliable)
	void AimTowardsCrosshair();

	//׼����Ļ�������
	UPROPERTY(EditDefaultsOnly, Category = "Cross Hair")
	FVector2D CrossHairLocationScale;

	UPROPERTY(EditDefaultsOnly, Category = "Cross Hair")
	float LineTraceRange;

};
