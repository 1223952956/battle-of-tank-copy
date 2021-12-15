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
#include "TankTrackStaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(TEXT("Aim Component"));
	TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(TEXT("Move Component"));

	LaunchSpeed = 3000.0f;

	FireRate = 1.0f;
	bIsFiring = false;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	Defence = 5.0f;

	MaxMoveSpeed = 1000.0f;
	MaxTurnSpeed = 30.0f;

	//bIsSwitching = false;

	//Ĭ��ai������
	AIControllerClass = ATankAIController::StaticClass();
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
		NewLocation += GetActorForwardVector() * MoveAxisValue * DeltaTime * MaxMoveSpeed;
		SetActorLocation(NewLocation);
		MoveForwardServer(NewLocation);
	}

	if (TurnAxisValue != 0.0f) {
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += TurnAxisValue * DeltaTime * MaxTurnSpeed;
		SetActorRotation(NewRotation);
		TurnRightServer(NewRotation);
	}

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("AimElevation"), this, &ATank::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("AimAzimuth"), this, &ATank::YawCamera);

	//PlayerInputComponent->BindAxis(TEXT("LeftTrackThrottle"), this, &ATank::LeftTrackThrottle);
	//PlayerInputComponent->BindAxis(TEXT("RightTrackThrottle"), this, &ATank::RightTrackThrottle);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::MoveForward);

	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &ATank::TurnRight);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::StartFire);

	PlayerInputComponent->BindAction(TEXT("SwitchPre"), IE_Pressed, this, &ATank::SwitchPreCannonType);
	PlayerInputComponent->BindAction(TEXT("SwitchNext"), IE_Pressed, this, &ATank::SwitchNextCannonType);
}

void ATank::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//���Ƶ�ǰ����ֵ��
	DOREPLIFETIME(ATank, CurrentHealth);
	DOREPLIFETIME(ATank, CannonTypeIndex);

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

void ATank::SetTrackReference(UTankTrackStaticMeshComponent* LeftTrackToSet, UTankTrackStaticMeshComponent* RightTrackToSet) {
	check(LeftTrackToSet != nullptr && RightTrackToSet != nullptr);
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

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
	SetActorRotation(NewRotation);
	TurnRightMulticast(NewRotation);
}

void ATank::TurnRightMulticast_Implementation(FRotator NewRotation) {
	if (GetLocalRole() < ROLE_AutonomousProxy)
		SetActorRotation(NewRotation);
}

void ATank::SwitchPreCannonType() {
	if (bIsFiring) return;

	bIsFiring = true;
	UWorld* World = GetWorld();
	check(World != nullptr);
	World->GetTimerManager().SetTimer(FiringTimer, this, &ATank::StopSwitchPre, FireRate, false);
	
	SwitchPreServer();

}

void ATank::StopSwitchPre() {
	bIsFiring = false;
}

void ATank::SwitchPreServer_Implementation() {
	CannonTypeIndex = CannonTypeIndex - 1 < 0 ? 0 : CannonTypeIndex - 1;
	ChangeCannon();
}

void ATank::SwitchNextCannonType() {
	//UE_LOG(LogTemp, Warning, TEXT("SwitchNextCannonType()"));

	if (bIsFiring) return;

	bIsFiring = true;
	UWorld* World = GetWorld();
	check(World != nullptr);
	World->GetTimerManager().SetTimer(FiringTimer, this, &ATank::StopSwitchNext, FireRate, false);

	SwitchNextServer();
}

void ATank::StopSwitchNext() {
	bIsFiring = false;
}

void ATank::SwitchNextServer_Implementation() {
	//UE_LOG(LogTemp, Warning, TEXT("SwitchNextServer()"));
	CannonTypeIndex = CannonTypeIndex + 1 >= CannonTypes.Num() ? CannonTypes.Num() - 1 : CannonTypeIndex + 1;
	ChangeCannon();
}

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

	check(GetWorld() != nullptr);
	AProjectile* spawnedProjectile = GetWorld()->SpawnActor<AProjectile>(CannonBlueprintArray[CannonTypeIndex]/*ProjectileBlueprint*/, spawnLocation, spawnRotation, spawnParameters);
	//spawnedProjectile->Launch(LaunchSpeed);
}

void ATank::OnRep_CurrentHealth()
{
	OnHealthUpdate();

}

void ATank::OnHealthUpdate() {
	//�ͻ���Ѫ����ʾ
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

	//������Ѫ����ʾ
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

