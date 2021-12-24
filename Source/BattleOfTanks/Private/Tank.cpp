// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "TankBarrelStaticMeshComponent.h"
#include "TankTurretStaticMeshComponent.h"
#include "Projectile.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "TankAIController.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Shield.h"
#include "Components/WidgetComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(TEXT("Aim Component"));
	TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(TEXT("Move Component"));
	//WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));

	LaunchSpeed = 3000.0f;

	FireRate = 1.0f;
	bIsFiring = false;

	MaxHealth = 500.0f;
	CurrentHealth = MaxHealth;

	Defence = 5.0f;

	MaxShieldSlotNum = 2;
	ShieldSlots.Init(nullptr, MaxShieldSlotNum);

	MaxShieldStorageNum = 8;
	ShieldStoraged.Init(nullptr, MaxShieldStorageNum);

	MaxMoveSpeed = 1000.0f;
	MaxTurnSpeed = 60.0f;

	//bIsSwitching = false;

	//默认ai控制类
	AIControllerClass = ATankAIController::StaticClass();

	//ShieldClass = AShield::StaticClass();
	
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MoveAxisValue != 0.0f) {
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorForwardVector() * MoveAxisValue * DeltaTime  * MaxMoveSpeed;
		SetActorLocation(NewLocation);
		MoveForwardServer(NewLocation);
	}

	if (TurnAxisValue != 0.0f) {
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += TurnAxisValue * DeltaTime * MaxTurnSpeed;
		//check(GEngine != nullptr);
		//FString str = FString::Printf(TEXT("TurnAxisValue : %f,  DeltaTime : %f, MaxTurnSpeed : %f"), TurnAxisValue, DeltaTime, MaxTurnSpeed);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, str);
		SetActorRotation(NewRotation);
		//check(GEngine != nullptr);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Trun Right Client"));
		TurnRightServer(NewRotation);
	}

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("TestTrigger"), IE_Pressed, this, &ATank::TestTrigger);

	PlayerInputComponent->BindAxis(TEXT("AimElevation"), this, &ATank::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("AimAzimuth"), this, &ATank::YawCamera);

	//PlayerInputComponent->BindAxis(TEXT("LeftTrackThrottle"), this, &ATank::LeftTrackThrottle);
	//PlayerInputComponent->BindAxis(TEXT("RightTrackThrottle"), this, &ATank::RightTrackThrottle);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::MoveForward);

	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &ATank::TurnRight);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::StartFire);

	PlayerInputComponent->BindAction(TEXT("SwitchPreCannon"), IE_Pressed, this, &ATank::SwitchPreCannonType);
	PlayerInputComponent->BindAction(TEXT("SwitchNextCannon"), IE_Pressed, this, &ATank::SwitchNextCannonType);

	PlayerInputComponent->BindAction(TEXT("EquipShield"), IE_Pressed, this, &ATank::EquipShield);
	PlayerInputComponent->BindAction(TEXT("UnloadShield"), IE_Pressed, this, &ATank::UnEquipShield);

	PlayerInputComponent->BindAction(TEXT("ChangeShieldSlot"), IE_Pressed, this, &ATank::InputChangeShieldSlot);
	PlayerInputComponent->BindAction(TEXT("SwitchPreShield"), IE_Pressed, this, &ATank::InputSwitchPreShield);
	PlayerInputComponent->BindAction(TEXT("SwitchNextShield"), IE_Pressed, this, &ATank::InputSwitchNextShield);
}

void ATank::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//复制属性
	DOREPLIFETIME(ATank, CurrentHealth);
	DOREPLIFETIME(ATank, CannonTypeIndex);
	//DOREPLIFETIME(ATank, ShieldSlots);
	//DOREPLIFETIME(ATank, ShieldStoraged);
	DOREPLIFETIME(ATank, Defence);
	DOREPLIFETIME(ATank, CampFlag);


}

void ATank::TestTrigger() {
	UE_LOG(LogTemp, Warning, TEXT("****************************TestTriggerBegin****************************"));

	for (int i = 0; i < ShieldStoraged.Num(); ++i) {
		FString str = ShieldStoraged[i] == nullptr ? TEXT("Empty") : ShieldStoraged[i]->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Field %d : %s"), i, *str);
	}

	//UE_LOG(LogTemp, Warning, TEXT("*****************************TestTriggerEnd*****************************"));
}

void ATank::AimAt(FVector HitLocation) {
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);

}

void ATank::SetBarrelAndTurretReference(UTankBarrelStaticMeshComponent* BarrelToSet, UTankTurretStaticMeshComponent* TurretToSet) {
	check(BarrelToSet != nullptr && TurretToSet != nullptr);
	Barrel = BarrelToSet;
	TankAimingComponent->SetBarrelAndTurretReference(BarrelToSet, TurretToSet);
}

void ATank::SetCameraReference(USceneComponent* AzimuthGimbalToSet, USceneComponent* SpringArmToSet) {
	check(AzimuthGimbalToSet != nullptr && SpringArmToSet != nullptr);
	AzimuthGimbalRef = AzimuthGimbalToSet;
	SpringArmRef = SpringArmToSet;
}

//void ATank::SetTrackReference(UTankTrackStaticMeshComponent* LeftTrackToSet, UTankTrackStaticMeshComponent* RightTrackToSet) {
//	check(LeftTrackToSet != nullptr && RightTrackToSet != nullptr);
//	LeftTrack = LeftTrackToSet;
//	RightTrack = RightTrackToSet;
//}

void ATank::PitchCamera(float AxisValue) {
	//UE_LOG(LogTemp, Warning, TEXT("PitchCamera call"));

	FRotator NewRotation = SpringArmRef->GetComponentRotation();
	NewRotation.Pitch += AxisValue;
	SpringArmRef->SetWorldRotation(NewRotation);
}

void ATank::YawCamera(float AxisValue) {
	//UE_LOG(LogTemp, Warning, TEXT("YawCamera call"));
	FRotator NewRotation = AzimuthGimbalRef->GetComponentRotation();
	NewRotation.Yaw += AxisValue;
	//UE_LOG(LogTemp, Warning, TEXT("NewRotationYaw: %f"), NewRotation.Yaw);
	AzimuthGimbalRef->SetWorldRotation(NewRotation);
}

void ATank::MoveForward(float AxisValue) {
	//TankMovementComponent->IntendMoveForward(AxisValue * CurrentHealth / MaxHealth);
	MoveAxisValue = AxisValue * CurrentHealth / MaxHealth;
}

void ATank::MoveForwardServer_Implementation(FVector NewLocation) {
	SetActorLocation(NewLocation);
	//UE_LOG(LogTemp, Warning, TEXT("ActorLocation : %s"), *GetActorLocation().ToString());
	MoveForwardMulticast(NewLocation);
}

void ATank::MoveForwardMulticast_Implementation(FVector NewLocation) {
	if (GetLocalRole() < ROLE_AutonomousProxy)
		SetActorLocation(NewLocation);
}

void ATank::TurnRight(float AxisValue) {
	TurnAxisValue = AxisValue;
}

void ATank::TurnRightServer_Implementation(FRotator NewRotation) {
	//UE_LOG(LogTemp, Warning, TEXT("TurnRightServer()"));
	SetActorRotation(NewRotation);
	TurnRightMulticast(NewRotation);
}

void ATank::TurnRightMulticast_Implementation(FRotator NewRotation) {
	if (GetLocalRole() < ROLE_AutonomousProxy)
		SetActorRotation(NewRotation);
}

void ATank::SwitchPreCannonType() {
	//if (bIsFiring) return;

	//bIsFiring = true;
	//UWorld* World = GetWorld();
	//check(World != nullptr);
	//World->GetTimerManager().SetTimer(FiringTimer, this, &ATank::StopFire, FireRate, false);
	//
	//SwitchPreServer();
	StartSwitch(-1);

}

void ATank::SwitchNextCannonType() {
	//UE_LOG(LogTemp, Warning, TEXT("SwitchNextCannonType()"));

	//if (bIsFiring) return;

	//bIsFiring = true;
	//UWorld* World = GetWorld();
	//check(World != nullptr);
	//World->GetTimerManager().SetTimer(FiringTimer, this, &ATank::StopFire, FireRate, false);

	//SwitchNextServer();
	StartSwitch(1);
}

void ATank::StartSwitch(int SwitchNum) {
	if (bIsFiring) return;

	bIsFiring = true;
	UWorld* World = GetWorld();
	check(World != nullptr);
	World->GetTimerManager().SetTimer(FiringTimer, this, &ATank::StopFire, FireRate, false);

	SwitchServer(SwitchNum);
}

void ATank::SwitchServer_Implementation(int SwitchNum) {
	int NewIndex = CannonTypeIndex + SwitchNum;
	if (NewIndex < 0) {
		NewIndex = CannonTypes.Num() + NewIndex;
	}
	NewIndex = NewIndex % CannonTypes.Num();
	CannonTypeIndex = NewIndex;
	ChangeCannon();

}

//void ATank::SwitchPreServer_Implementation() {
//	CannonTypeIndex = CannonTypeIndex - 1 < 0 ? 0 : CannonTypeIndex - 1;
//	ChangeCannon();
//}
//
//
//
//void ATank::SwitchNextServer_Implementation() {
//	//UE_LOG(LogTemp, Warning, TEXT("SwitchNextServer()"));
//	CannonTypeIndex = CannonTypeIndex + 1 >= CannonTypes.Num() ? CannonTypes.Num() - 1 : CannonTypeIndex + 1;
//	ChangeCannon();
//}

void ATank::OnRep_CannonTypeIndex() {
	if (GetLocalRole() == ROLE_AutonomousProxy) {
		if (GEngine) {
			FString str = FString::Printf(TEXT("cannon %d have %d left"), CannonTypeIndex, CannonTypes[CannonTypeIndex]);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, str);
			
		}

	}
	ChangeCannon();
}

void ATank::ChangeCannon() {
	AProjectile* Cannon = CannonBlueprintArray[CannonTypeIndex].GetDefaultObject();
	if (Cannon) {
		LaunchSpeed = Cannon->Speed;
	}
}

void ATank::ReduceCannonNum() {
	CannonTypes[CannonTypeIndex] -= 1;
}

void ATank::ReduceCannonNumMulticast_Implementation() {
	ReduceCannonNum();
}


void ATank::StartFire() {
	if (bIsFiring || CannonTypes[CannonTypeIndex] <= 0) return;

	bIsFiring = true;
	UWorld* World = GetWorld();
	check(World != nullptr);
	World->GetTimerManager().SetTimer(FiringTimer, this, &ATank::StopFire, FireRate, false);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticleSystem, Barrel->GetSocketLocation(TEXT("Projectile")), FRotator(0.0f), true);
	HandleFire();
}

void ATank::StopFire() {
	bIsFiring = false;
}

void ATank::HandleFire_Implementation() {
	//UE_LOG(LogTemp, Warning, TEXT("Tank Fire !!!") );
	ReduceCannonNumMulticast();

	check(Barrel != nullptr);

	FVector spawnLocation = Barrel->GetSocketLocation(TEXT("Projectile"));
	//UE_LOG(LogTemp, Warning, TEXT("spawnLocation: %s"), *spawnLocation.ToString());
	FRotator spawnRotation = Barrel->GetSocketRotation(TEXT("Projectile"));

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	UWorld* World = GetWorld();
	check(World != nullptr);
	AProjectile* spawnedProjectile = World->SpawnActor<AProjectile>(CannonBlueprintArray[CannonTypeIndex]/*ProjectileBlueprint*/, spawnLocation, spawnRotation, spawnParameters);
	//spawnedProjectile->Launch(LaunchSpeed);
}

void ATank::OnRep_CurrentHealth()
{
	OnHealthUpdate();

}

void ATank::OnHealthUpdate() {
	//客户端血量显示
	if (IsLocallyControlled()) {
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//服务器血量显示
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		UE_LOG(LogTemp, Warning,  TEXT("%s"), *healthMessage);
	}

}

void ATank::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

float ATank::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float TrueDamage = DamageTaken - Defence > 0 ? DamageTaken - Defence : DamageTaken / 4;
	float damageApplied = CurrentHealth - TrueDamage;
	SetCurrentHealth(damageApplied);
	
	return damageApplied;
}

void ATank::AddCannonServer(int32 index, int32 Num) {
	UE_LOG(LogTemp, Warning, TEXT("AddCannonServer(), TypeName : %d, Num : %d"), index, Num);

	AddCannonMulticast(index, Num);
}

void ATank::AddCannonMulticast_Implementation(int32 index, int32 Num) {
	//if(GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("AddCannonMulticast()"));
	if (index < 0 && CannonTypes.Num() <= index) return;

	CannonTypes[index] += Num;
	
	if (GetLocalRole() == ROLE_AutonomousProxy) {
		FString str = FString::Printf(TEXT("Left Cannon : %d"), CannonTypes[index]);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, str);
	}
}

void ATank::InputChangeShieldSlot() {
	ShieldSlotIndex += 1;
	ShieldSlotIndex %= MaxShieldSlotNum;

	FString str = FString::Printf(TEXT("ShieldSlotIndex : %d"), ShieldSlotIndex);
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, str);
}

void ATank::InputSwitchPreShield() {
	ShieldStorageIndex -= 1;
	if (ShieldStorageIndex < 0)
		ShieldStorageIndex += MaxShieldStorageNum;

	FString str = FString::Printf(TEXT("ShieldStorageIndex : %d"), ShieldStorageIndex);
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, str);
}

void ATank::InputSwitchNextShield() {
	ShieldStorageIndex += 1;
	ShieldStorageIndex %= MaxShieldStorageNum;

	FString str = FString::Printf(TEXT("ShieldStorageIndex : %d"), ShieldStorageIndex);
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, str);
}

void ATank::ChangeDefenceServer_Implementation(float InNum) {
	Defence += InNum;
}

void ATank::EquipShield() {
	if (!ShieldStoraged[ShieldStorageIndex]) return;
	if (ShieldSlots[ShieldSlotIndex] && ShieldSlots[ShieldSlotIndex] != ShieldStoraged[ShieldStorageIndex])
		UnEquipShield();

	ShieldStoraged[ShieldStorageIndex]->Equip(ShieldSlotIndex);
	//if (ShieldSlot_1) {
	//	ShieldSlot_1->Equip();
	//	return;
	//}

	//UE_LOG(LogTemp, Warning, TEXT("ATank::EquipShield()"));

	//UWorld* World = GetWorld();
	//check(World != nullptr);

	//FActorSpawnParameters spawnParameters;
	//spawnParameters.Instigator = GetInstigator();
	//spawnParameters.Owner = this;

	//FVector SpawnLocation = GetActorLocation() + FVector::ForwardVector * 100;
	//FRotator SpawnRotation = FRotator(0,90,0);

	//ShieldSlot_1 = World->SpawnActor<AShield>(ShieldClass, SpawnLocation, SpawnRotation, spawnParameters);
	//ShieldSlot_1->AttachToTank(this);
	//ShieldSlot_1->Equip();


}

void ATank::UnEquipShield() {
	if (!ShieldSlots[ShieldSlotIndex]) return;

	ShieldSlots[ShieldSlotIndex]->UnEquip();
	//UE_LOG(LogTemp, Warning, TEXT("ATank::UnloadShield()"));
	//if (!ShieldSlot_1) return;
	//ShieldSlot_1->UnEquip();

}

void ATank::OnRep_Defence() {
	if (GetLocalRole() == ROLE_AutonomousProxy) {
		FString str = FString::Printf(TEXT("You have %f Defence now."), Defence);
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, str);
	}
	
}

