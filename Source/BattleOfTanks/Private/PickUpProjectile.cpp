// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"

APickUpProjectile::APickUpProjectile() {
	
	Amount = 5;

}

void APickUpProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
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

void APickUpProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherComp != nullptr && OtherComp->GetName() == TEXT("Tank")) {
		UE_LOG(LogTemp, Warning, TEXT("OnOverlapEnd()"));
	}
}