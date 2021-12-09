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

	//获取准星正向射线击中的位置, 如果为真, 则击中
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	//控制炮台瞄准准星的指向
	//UFUNCTION(Server, unreliable)
	void AimTowardsCrosshair();

	//准星屏幕坐标比例
	UPROPERTY(EditDefaultsOnly, Category = "Cross Hair")
	FVector2D CrossHairLocationScale;

	UPROPERTY(EditDefaultsOnly, Category = "Cross Hair")
	float LineTraceRange;

};
