// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include "Tank.h"

// Sets default values
AShield::AShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxLifeTime = 3.0f;
	CurrentLifeTime = MaxLifeTime;
	bIsEquipped = false;
	DefenceValue = 10.0f;

	TestMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TestMesh(TEXT("/Game/StarterContent/Architecture/Wall_400x200.Wall_400x200"));
	if (TestMesh.Succeeded()) {
		TestMeshComponent->SetStaticMesh(TestMesh.Object);
	}
	TestMeshComponent->SetCollisionProfileName("NoCollision");
	RootComponent = TestMeshComponent;
}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsEquipped) {
		CurrentLifeTime -= DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("current life time: %f"), CurrentLifeTime);
		if (CurrentLifeTime <= 0) {
			UnEquip();
			UnAttach();
			Destroy();
		}
	}

}

void AShield::AttachToTank(ATank* InOwnerTank) {
	check(InOwnerTank != nullptr);

	OwnerTank = InOwnerTank;
}

void AShield::UnAttach() {
	if (!OwnerTank) return;
	OwnerTank->ShieldSlot_1 = nullptr;
}

void AShield::Equip() {
	if (!OwnerTank || bIsEquipped) return;
	
	OwnerTank->Defence += DefenceValue;
	bIsEquipped = true;

	FString str = FString::Printf(TEXT("Tank have %f defence now"), OwnerTank->Defence);
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, str);
}

void AShield::UnEquip() {
	if (!OwnerTank || !bIsEquipped) return;

	OwnerTank->Defence -= DefenceValue;
	bIsEquipped = false;

	FString str = FString::Printf(TEXT("Tank have %f defence now"), OwnerTank->Defence);
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, str);
}

