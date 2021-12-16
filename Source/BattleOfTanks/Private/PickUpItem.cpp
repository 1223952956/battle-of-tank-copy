// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Tank.h"

// Sets default values
APickUpItem::APickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	//创建碰撞组件并设为根组件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(200.0f);
	RootComponent = CollisionComp;
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	IdleParticlesComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdelParticlesComponent"));
	IdleParticlesComp->SetupAttachment(RootComponent);

	bRotate = false;
	RotationRate = 45.0f;

	Amount = 1;
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APickUpItem::OnOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &APickUpItem::OnOverlapEnd);
}

// Called every frame
void APickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotate) {
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}
}

void APickUpItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherComp != nullptr && OtherComp->GetName() == TEXT("Tank")) {
		UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin()"));
		
		if (OverlapParticles/* && GetLocalRole() < ROLE_Authority*/) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.0f), true);
		}

		if (GetLocalRole() == ROLE_Authority) {
			ATank* OtherTank = Cast<ATank>(OtherActor);
			if (OtherTank) {
				OtherTank->AddCannonServer(TypeNum, Amount);
			}
			Destroy();
		}
		
	}
	
}

void APickUpItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherComp != nullptr && OtherComp->GetName() == TEXT("Tank")) {
		UE_LOG(LogTemp, Warning, TEXT("OnOverlapEnd()"));
	}
	
}