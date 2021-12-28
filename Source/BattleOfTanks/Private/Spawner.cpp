// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus"));
	if (DefaultMesh.Succeeded()) {
		StaticMeshComponent->SetStaticMesh(DefaultMesh.Object);
	}
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	ActorToSpawn = AActor::StaticClass();
	SpawnInterval = 5.0f;
	TimeToNextSpawn = SpawnInterval;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TimeToNextSpawn <= 0) {
		TimeToNextSpawn = SpawnInterval;
		SpawnActor();
	}
	TimeToNextSpawn -= DeltaTime;

}

void ASpawner::SpawnActor() {
	if (GetLocalRole() < ROLE_Authority) return;
	UWorld* World = GetWorld();
	check(World != nullptr);

	FVector SpawnLocation = GetActorLocation() + GetActorUpVector() * 200.0f;
	FRotator SpawnRotation;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	World->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation, SpawnParameters);
}

