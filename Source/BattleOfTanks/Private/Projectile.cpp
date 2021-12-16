
// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Tank.h"
#include "Engine/Engine.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	//SetLifeSpan(3.0f);

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;

	//Speed = 10000.0f;

	//创建碰撞组件并设为根组件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComp->InitSphereRadius(37.5f);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->SetupAttachment(RootComponent);
	RootComponent = CollisionComp;
	CollisionComp->SetNotifyRigidBodyCollision(true);
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->SetSimulatePhysics(true);
	
	//CollisionComp->bAutoActivate = false;

	
		

	//创建静态网格组件
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	StaticMeshComp->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (DefaultMesh.Succeeded()) {
		StaticMeshComp->SetStaticMesh(DefaultMesh.Object);
		StaticMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -12.5f));
		StaticMeshComp->SetRelativeScale3D(FVector(0.25f));
	}
	//StaticMeshComp->SetCollisionProfileName(TEXT("Projectile"));
	//StaticMeshComp->SetGenerateOverlapEvents(true);
	//StaticMeshComp->SetSimulatePhysics(true);
	//RootComponent = StaticMeshComp;

	//指定粒子效果
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (DefaultExplosionEffect.Succeeded()) {
		ExplosionEffect = DefaultExplosionEffect.Object;
	}

	//创建子弹运动组件
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	UE_LOG(LogTemp, Warning, TEXT("Speed in constructor : %f"), Speed);
	ProjectileMovementComp->InitialSpeed = Speed;
	//ProjectileMovementComp->SetVelocityInLocalSpace(FVector::ForwardVector *);
	ProjectileMovementComp->SetUpdatedComponent(RootComponent);
	ProjectileMovementComp->MaxSpeed = 100000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = false;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	//ProjectileMovementComp->bAutoActivate = false;



}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Projectile spawned at %s."), *GetActorLocation().ToString());
	//check(GEngine != nullptr);
	//FString str = TEXT("Projectile Spawned at ");
	//str += GetActorLocation().ToString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, str);
	if (GetLocalRole() == ROLE_Authority) {

		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileOverlapPawn);
		CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnProjectileHitGround);
	}
	UE_LOG(LogTemp, Warning, TEXT("Speed in BeginPlay() : %f"), Speed);
	UE_LOG(LogTemp, Warning, TEXT("Projectile velocity : %s"), *(Speed * GetActorForwardVector()).ToString());
	//ProjectileMovementComp->Velocity = Speed * GetActorForwardVector();
	//ProjectileMovementComp->SetVelocityInLocalSpace(Speed * GetActorForwardVector());
	SetLifeSpan(3.0f);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Projectile position : %s"), *GetActorLocation().ToString());
}

void AProjectile::Destroyed()
{
	FVector spawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
}

void AProjectile::OnProjectileOverlapPawn(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Pawn !!!"));
	if (OtherActor && OtherActor->IsA(ATank::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigator()->Controller, this, DamageType);

		//if (OtherComp) {
		//	UE_LOG(LogTemp, Warning, TEXT("Velocity : %s"), *ProjectileMovementComp->Velocity.ToString());
		//	OtherComp->AddImpulseAtLocation(ProjectileMovementComp->Velocity, SweepResult.ImpactPoint);
		//	OtherComp->AddForceAtLocation(ProjectileMovementComp->Velocity * 300000.0f, SweepResult.ImpactPoint);
		//
		//}

			
	}

	Destroy();
}

void AProjectile::OnProjectileHitGround(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogTemp, Warning, TEXT("Hit Ground !!!"));
	Destroy();
}


//void AProjectile::Launch(float LaunchSpeed) {
//	
//	ProjectileMovementComp->Velocity = LaunchSpeed * GetActorForwardVector();
//	UE_LOG(LogTemp, Warning, TEXT("Velocity : %s"), *ProjectileMovementComp->Velocity.ToString());
//	ProjectileMovementComp->Activate();
//	//CollisionComp->Activate();
//}
