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
/// ��������
/// </summary>
public:
	//UFUNCTION()
	void TestTrigger();

	void PitchCamera(float AxisValue);

	void YawCamera(float AxisValue);

/// <summary>
/// ��������
/// </summary>
public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetBarrelAndTurretReference(UTankBarrelStaticMeshComponent* BarrelToSet, UTankTurretStaticMeshComponent* TurretToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetCameraReference(USceneComponent* AzimuthGimbalToSet, USceneComponent* SpringArmToSet);

	//UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//TSubclassOf<AProjectile> ProjectileBlueprint;

/// <summary>
/// ���
/// </summary>
protected:
	UPROPERTY(BlueprintReadOnly)
	UTankAimingComponent* TankAimingComponent;

	//UPROPERTY(BlueprintReadOnly)
	//UTankMovementComponent* TankMovementComponent;

	//UPROPERTY(EditDefaultsOnly, Category = "Component")
	//USphereComponent* PickUpDetectComp;
private:
	//��λ�����������
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USceneComponent* AzimuthGimbalRef;

	//���ɱ�����
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USceneComponent* SpringArmRef;

	//�ڹ�����
	UTankBarrelStaticMeshComponent* Barrel;


/// <summary>
/// ����
/// </summary>
public:
	//̹����׼
	void AimAt(FVector HitLocation);

	//�ɷ�����ӵ���������
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Fire")
	TArray<TSubclassOf<AProjectile>> CannonBlueprintArray;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Fire")
	UParticleSystem* FireParticleSystem;

	//�ӵ�����(�����ӵ����͵���)
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float LaunchSpeed;

	//������
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Fire")
	float FireRate;

	bool bIsLoading;
	
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void StopFire();

	//�ڷ����������ڵ�
	UFUNCTION(Server, Reliable)
	void HandleFire();

	//���������������ļ�ʱ��
	FTimerHandle FiringTimer;

	//װ��UI����
	UFUNCTION(BlueprintNativeEvent)
	void DisplayLoadCannon();

/// <summary>
/// ��ҩ
/// </summary>
public:
	//��ŵ�ҩ�������Ӧ�ĸ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup|Cannon")
	TArray<int32> CannonTypes;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_CannonTypeIndex)
	int32 CannonTypeIndex;

	//��ӵ�ҩ����, �ɿ�ʰȡ�����
	UFUNCTION(/*Server, Reliable*/)
	void AddCannonServer(int32 index, int32 Num);

	UFUNCTION(NetMulticast, Reliable)
	void AddCannonMulticast(int32 index, int32 Num);

	void SwitchPreCannonType();
	void SwitchNextCannonType();

	//���ĵ�ҩ��������
	void StartSwitch(int SwitchNum);

	UFUNCTION(Server, Reliable)
	void SwitchServer(int SwitchNum);

	//UFUNCTION(Server, Reliable)
	//void SwitchPreServer();
	//UFUNCTION(Server, Reliable)
	//void SwitchNextServer();

	UFUNCTION()
	void OnRep_CannonTypeIndex();

	//���������������ĵ�ҩ����
	void ChangeCannon();

	//����ʱ��ҩ����
	void ReduceCannonNum();

	UFUNCTION(NetMulticast, Reliable)
	void ReduceCannonNumMulticast();

	//UFUNCTION(BlueprintCallable, Category = "Cannon")
	//int32 GetCurrentCannonNum();

/// <summary>
/// Ѫ��
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

	//�ڷ�����������������
	UFUNCTION(Server, Reliable)
	void DieServer();

	//�ڷ������Ƴ��������Ŀ�����
	UFUNCTION(BlueprintNativeEvent)
	void RemoveController();

	UFUNCTION(BlueprintNativeEvent)
	void DisplayDefeatUI();

protected:
	//��ʾѪ������
	void OnHealthUpdate();

/// <summary>
/// �ƶ�
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
/// ����
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
/// ����
/// </summary>
public:
	//��Ӫ��־
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated ,Category = "Flag")
	int32 CampFlag;

};
