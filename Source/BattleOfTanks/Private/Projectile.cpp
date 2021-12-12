// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;

	//创建碰撞组件并指定为根组件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	CollisionComp->InitSphereRadius(37.5f);
	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = CollisionComp;


	
		

	//创建静态网格组件
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	StaticMeshComp->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (DefaultMesh.Succeeded()) {
		StaticMeshComp->SetStaticMesh(DefaultMesh.Object);
		StaticMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -12.5f));
		StaticMeshComp->SetRelativeScale3D(FVector(0.25f));
	}

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->SetUpdatedComponent(RootComponent);
	ProjectileMovementComp->InitialSpeed = 10000.0f;
	ProjectileMovementComp->MaxSpeed = 10000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;




}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Projectile spawned at %s."), *GetActorLocation().ToString());
	check(GEngine != nullptr);
	FString str = TEXT("Projectile Spawned at ");
	str += GetActorLocation().ToString();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, str);
	if (GetLocalRole() == ROLE_Authority) {
		CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnProjectileImpact);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Destroyed()
{
	FVector spawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
}

void AProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hit !!!"));
	if (OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this, DamageType);
		HitComponent->AddImpulseAtLocation(ProjectileMovementComp->Velocity, Hit.ImpactPoint);
	}

	Destroy();
}
