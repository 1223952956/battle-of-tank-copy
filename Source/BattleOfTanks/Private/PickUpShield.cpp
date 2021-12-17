// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpShield.h"
#include "Shield.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Components/SphereComponent.h"

APickUpShield::APickUpShield() {

	ShieldClass = AShield::StaticClass();
}

//void APickUpShield::BeginPlay() {
//	Super::BeginPlay();
//
//	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APickUpShield::OnOverlapBegin);
//	//CollisionComp->OnComponentEndOverlap.AddDynamic(this, &APickUpShield::OnOverlapEnd);
//}


void APickUpShield::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ATank* OverlapTank = Cast<ATank>(OtherActor);
	if (!OverlapTank) return;

	FVector SpawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this, OverlapParticles, SpawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);

	int32 n = OverlapTank->ShieldStoraged.Num();

	for (int i = 0; i < n; ++i) {
		if (!OverlapTank->ShieldStoraged[i]) {

			UWorld* World = GetWorld();
			check(World != nullptr);
			AShield* Shield = World->SpawnActor<AShield>(ShieldClass);

			OverlapTank->ShieldStoraged[i] = Shield;
			Shield->AttachToTank(OverlapTank, i);
			if (GetLocalRole() == ROLE_Authority) {
				Destroy();
			}

			break;
		}
	}






}

void APickUpShield::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}

//void APickUpShield::PickUpMulticast_Implementation(FString InShieldName) {
//
//}