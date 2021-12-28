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

/// <summary>
/// 基本输入
/// </summary>
public:
	//UFUNCTION()
	void TestTrigger();

	void PitchCamera(float AxisValue);

	void YawCamera(float AxisValue);

/// <summary>
/// 基本设置
/// </summary>
public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetBarrelAndTurretReference(UTankBarrelStaticMeshComponent* BarrelToSet, UTankTurretStaticMeshComponent* TurretToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetCameraReference(USceneComponent* AzimuthGimbalToSet, USceneComponent* SpringArmToSet);

	//UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//TSubclassOf<AProjectile> ProjectileBlueprint;

/// <summary>
/// 组件
/// </summary>
protected:
	UPROPERTY(BlueprintReadOnly)
	UTankAimingComponent* TankAimingComponent;

	//UPROPERTY(BlueprintReadOnly)
	//UTankMovementComponent* TankMovementComponent;

	//UPROPERTY(EditDefaultsOnly, Category = "Component")
	//USphereComponent* PickUpDetectComp;
private:
	//方位角万向节引用
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USceneComponent* AzimuthGimbalRef;

	//弹簧臂引用
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USceneComponent* SpringArmRef;

	//炮管引用
	UTankBarrelStaticMeshComponent* Barrel;


/// <summary>
/// 开火
/// </summary>
public:
	//坦克瞄准
	void AimAt(FVector HitLocation);

	//可发射的子弹类型数组
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Fire")
	TArray<TSubclassOf<AProjectile>> CannonBlueprintArray;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Fire")
	UParticleSystem* FireParticleSystem;

	//子弹速率(根据子弹类型调整)
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float LaunchSpeed;

	//开火间隔
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Fire")
	float FireRate;

	bool bIsLoading;
	
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void StopFire();

	//在服务器生成炮弹
	UFUNCTION(Server, Reliable)
	void HandleFire();

	//用于生成射击间隔的计时器
	FTimerHandle FiringTimer;

	//装弹UI表现
	UFUNCTION(BlueprintNativeEvent)
	void DisplayLoadCannon();

/// <summary>
/// 弹药
/// </summary>
public:
	//存放弹药种类和相应的个数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup|Cannon")
	TArray<int32> CannonTypes;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_CannonTypeIndex)
	int32 CannonTypeIndex;

	//添加弹药函数, 由可拾取物调用
	UFUNCTION(/*Server, Reliable*/)
	void AddCannonServer(int32 index, int32 Num);

	UFUNCTION(NetMulticast, Reliable)
	void AddCannonMulticast(int32 index, int32 Num);

	void SwitchPreCannonType();
	void SwitchNextCannonType();

	//更改弹药类型索引
	void StartSwitch(int SwitchNum);

	UFUNCTION(Server, Reliable)
	void SwitchServer(int SwitchNum);

	//UFUNCTION(Server, Reliable)
	//void SwitchPreServer();
	//UFUNCTION(Server, Reliable)
	//void SwitchNextServer();

	UFUNCTION()
	void OnRep_CannonTypeIndex();

	//根据类型索引更改弹药种类
	void ChangeCannon();

	//发射时弹药减少
	void ReduceCannonNum();

	UFUNCTION(NetMulticast, Reliable)
	void ReduceCannonNumMulticast();

	//UFUNCTION(BlueprintCallable, Category = "Cannon")
	//int32 GetCurrentCannonNum();

/// <summary>
/// 血量
/// </summary>
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup|Health")
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

	//在服务器调用死亡函数
	UFUNCTION(Server, Reliable)
	void DieServer();

	//在服务器移除已死亡的控制器
	UFUNCTION(BlueprintNativeEvent)
	void RemoveController();

	UFUNCTION(BlueprintNativeEvent)
	void DisplayDefeatUI();

protected:
	//显示血量更新
	void OnHealthUpdate();

/// <summary>
/// 移动
/// </summary>
public:
	void MoveForward(float AxisValue);

	void TurnRight(float AxisValue);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup|Movement")
	float MaxMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Movement")
	float MaxTurnSpeed;

	UFUNCTION(Server, Unreliable)
	void MoveForwardServer(FVector NewLocation);

	UFUNCTION(NetMulticast, Unreliable)
	void MoveForwardMulticast(FVector NewLocation);

	UFUNCTION(Server, Unreliable)
	void TurnRightServer(FRotator NewRotation);

	UFUNCTION(NetMulticast, Unreliable)
	void TurnRightMulticast(FRotator NewRotation);

private:
	float MoveAxisValue;

	float TurnAxisValue;

/// <summary>
/// 护盾
/// </summary>
public:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Defence, Category = "Setup|Defence")
	float Defence;

	UFUNCTION(Server, Reliable)
	void ChangeDefenceServer(float InNum);

	UPROPERTY(BlueprintReadOnly, Category = "Defence")
	TArray<AShield*> ShieldSlots;

	UPROPERTY(BlueprintReadOnly, Category = "Defence")
	TArray<AShield*> ShieldStoraged;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Defence")
	int32 MaxShieldSlotNum;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Defence")
	int32 MaxShieldStorageNum;

	UPROPERTY(BlueprintReadOnly)
	int32 ShieldStorageIndex;

	UFUNCTION(BlueprintNativeEvent)
	void DisplaySelectedSlot(int32 InSlotIndex);
	
	UFUNCTION(BlueprintNativeEvent)
	void DisplayShieldStorage();

	UFUNCTION(BlueprintNativeEvent)
	void DisplaySelectedStorage();

	//AShield* ShieldSlot_1;
	//AShield* ShieldSlot_2;

protected:
	void InputChangeShieldSlot();

	void InputSwitchPreShield();
	void InputSwitchNextShield();

	void EquipShield();
	void UnEquipShield();

	UFUNCTION()
	void OnRep_Defence();

	//void OnRep_ShieldSlots();

private:
	int32 ShieldSlotIndex;

	//UPROPERTY(EditDefaultsOnly, Category = "Defence")
	//TSubclassOf<AShield> ShieldClass;

/// <summary>
/// 杂项
/// </summary>
public:
	//阵营标志
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated ,Category = "Flag")
	int32 CampFlag;

};
