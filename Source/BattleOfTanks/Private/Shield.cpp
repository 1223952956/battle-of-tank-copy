
// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include "Tank.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AShield::AShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = true;

	MaxLifeTime = 3.0f;
	//CurrentLifeTime = MaxLifeTime;
	bIsEquipped = false;
	DefenceValue = 10.0f;

	//TestMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestMesh"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> TestMesh(TEXT("/Game/StarterContent/Architecture/Wall_400x200.Wall_400x200"));
	//if (TestMesh.Succeeded()) {
	//	TestMeshComponent->SetStaticMesh(TestMesh.Object);
	//}
	//TestMeshComponent->SetCollisionProfileName("NoCollision");
	//RootComponent = TestMeshComponent;
}

//void AShield::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AShield, OwnerTank);
//}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
	CurrentLifeTime = MaxLifeTime;
}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsEquipped) {
		CurrentLifeTime -= DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("current life time: %f"), CurrentLifeTime);
		if (CurrentLifeTime <= 0) {
			UnEquip();
			UnAttach();
			Destroy();
		}
	}

}

void AShield::AttachToTank(ATank* InOwnerTank, int32 InStorageIndex) {
	if (!InOwnerTank) return;

	OwnerTank = InOwnerTank;
	StorageIndex = InStorageIndex;
	OwnerTank->ShieldStoraged[StorageIndex] = this;
}

void AShield::UnAttach() {
	if (!OwnerTank) return;

	OwnerTank->ShieldStoraged[StorageIndex] = nullptr;
	StorageIndex = 0;
	OwnerTank = nullptr;
}

void AShield::Equip(int32 InSlotIndex) {
	if (!OwnerTank || bIsEquipped) return;
	
	SlotIndex = InSlotIndex;
	OwnerTank->ShieldSlots[SlotIndex] = this;
	OwnerTank->ChangeDefenceServer(DefenceValue);
	bIsEquipped = true;
	UE_LOG(LogTemp, Warning, TEXT("Equip()"));

	//FString str = FString::Printf(TEXT("Tank have %f defence now"), OwnerTank->Defence);
	//check(GEngine != nullptr);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, str);
	//UE_LOG(LogTemp, Warning, TEXT("Tank have %f defence now"), OwnerTank->Defence);
}

void AShield::UnEquip() {
	if (!OwnerTank || !bIsEquipped) return;

	bIsEquipped = false;
	OwnerTank->ChangeDefenceServer(-DefenceValue);
	OwnerTank->ShieldSlots[SlotIndex] = nullptr;
	SlotIndex = 0;

	//FString str = FString::Printf(TEXT("Tank have %f defence now"), OwnerTank->Defence);
	//check(GEngine != nullptr);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, str);
	//UE_LOG(LogTemp, Warning, TEXT("Tank have %f defence now"), OwnerTank->Defence);
}

