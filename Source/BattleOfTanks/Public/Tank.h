// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankAimingComponent;
class UTankBarrelStaticMeshComponent;
class UTankTurretStaticMeshComponent;
class AProjectile;
class UTankMovementComponent;
class USphereComponent;
class AShield;
class UWidgetComponent;

UCLASS()
class BATTLEOFTANKS_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//基本输入
public:
	//UFUNCTION()
	void TestTrigger();

	void PitchCamera(float AxisValue);

	void YawCamera(float AxisValue);

	void MoveForward(float AxisValue);


	void TurnRight(float AxisValue);



//
public:
	//坦克瞄准
	void AimAt(FVector HitLocation);

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;



	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetBarrelAndTurretReference(UTankBarrelStaticMeshComponent* BarrelToSet, UTankTurretStaticMeshComponent* TurretToSet);


	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetCameraReference(USceneComponent* AzimuthGimbalToSet, USceneComponent* SpringArmToSet);


//组件
protected:
	UPROPERTY(BlueprintReadOnly)
	UTankAimingComponent* TankAimingComponent;

	UPROPERTY(BlueprintReadOnly)
	UTankMovementComponent* TankMovementComponent;

	//UPROPERTY(EditDefaultsOnly, Category = "Component")
	//USphereComponent* PickUpDetectComp;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USceneComponent* AzimuthGimbalRef;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USceneComponent* SpringArmRef;

	//本地炮管指针
	UTankBarrelStaticMeshComponent* Barrel;


//开火
public:
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TArray<TSubclassOf<AProjectile>> CannonBlueprintArray;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	UParticleSystem* FireParticleSystem;

	//子弹速率(不确定)
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float LaunchSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float FireRate;

	bool bIsFiring;
	
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void StopFire();

	//生成投射物的服务器函数
	UFUNCTION(Server, Reliable)
	void HandleFire();

	//用于生成射击间隔的计时器
	FTimerHandle FiringTimer;

//弹药
private:
	//存放弹药种类和相应的个数
	UPROPERTY(EditDefaultsOnly, Category = "Cannon")
	TArray<int32> CannonTypes;

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_CannonTypeIndex, Category = "Cannon")
	int32 CannonTypeIndex;


public:
	//添加弹药函数, 由可拾取物调用
	UFUNCTION(/*Server, Reliable*/)
	void AddCannonServer(int32 index, int32 Num);

	UFUNCTION(NetMulticast, Reliable)
	void AddCannonMulticast(int32 index, int32 Num);

	

	void SwitchPreCannonType();
	void SwitchNextCannonType();

	void StartSwitch(int SwitchNum);

	UFUNCTION(Server, Reliable)
	void SwitchServer(int SwitchNum);

	//UFUNCTION(Server, Reliable)
	//void SwitchPreServer();
	//UFUNCTION(Server, Reliable)
	//void SwitchNextServer();

	UFUNCTION()
	void OnRep_CannonTypeIndex();

	void ChangeCannon();

	void ReduceCannonNum();

	UFUNCTION(NetMulticast, Reliable)
	void ReduceCannonNumMulticast();

	//UFUNCTION(BlueprintCallable, Category = "Cannon")
	//int32 GetCurrentCannonNum();

//血量
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;



	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(Server, Reliable)
	void DieServer();

protected:
	void OnHealthUpdate();

//移动
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	float MaxMoveSpeed;

	float MaxTurnSpeed;

	float MoveAxisValue;

	float TurnAxisValue;

	UFUNCTION(Server, Unreliable)
	void MoveForwardServer(FVector NewLocation);

	UFUNCTION(NetMulticast, Unreliable)
	void MoveForwardMulticast(FVector NewLocation);

	UFUNCTION(Server, Unreliable)
	void TurnRightServer(FRotator NewRotation);

	UFUNCTION(NetMulticast, Unreliable)
	void TurnRightMulticast(FRotator NewRotation);


//护盾
public:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Defence, Category = "Defence")
	float Defence;

	UFUNCTION(Server, Reliable)
	void ChangeDefenceServer(float InNum);

	UPROPERTY(BlueprintReadOnly, Category = "Defence")
	TArray<AShield*> ShieldSlots;

	UPROPERTY(BlueprintReadOnly, Category = "Defence")
	TArray<AShield*> ShieldStoraged;

	UPROPERTY(EditDefaultsOnly, Category = "Defence")
	int32 MaxShieldSlotNum;

	UPROPERTY(EditDefaultsOnly, Category = "Defence")
	int32 MaxShieldStorageNum;

	void InputChangeShieldSlot();

	void InputSwitchPreShield();
	void InputSwitchNextShield();
	//AShield* ShieldSlot_1;

	//AShield* ShieldSlot_2;

protected:
	void EquipShield();

	void UnEquipShield();

	UFUNCTION()
	void OnRep_Defence();

	//void OnRep_ShieldSlots();

private:
	//UPROPERTY(EditDefaultsOnly, Category = "Defence")
	//TSubclassOf<AShield> ShieldClass;

	int32 ShieldSlotIndex;

	int32 ShieldStorageIndex;

//标志
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated ,Category = "Flag")
	int32 CampFlag;

//UI
public:	
	UFUNCTION(BlueprintNativeEvent)
	void DisplayDefeatUI();
};
