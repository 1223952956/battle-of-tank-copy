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
	//获取准星正向射线击中的位置, 如果为真, 则击中
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	//控制炮台瞄准准星的指向
	void AimTowardsCrosshair();

	//准星屏幕坐标比例
	UPROPERTY(EditDefaultsOnly, Category = "Cross Hair")
	FVector2D CrossHairLocationScale;



};
