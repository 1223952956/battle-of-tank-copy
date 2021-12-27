// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrackStaticMeshComponent.h"

UTankTrackStaticMeshComponent::UTankTrackStaticMeshComponent() {
	//SetIsReplicated(true);
	//假设坦克40吨
	TrackMaxDrivingForce = 400000;
}

void UTankTrackStaticMeshComponent::SetThrottle(float Throttle) {
	//auto Name = GetName();
	//UE_LOG(LogTemp, Warning, TEXT("%s Speed: %f"), *Name, Throttle);
	UWorld* World = GetWorld();
	check(World != nullptr);
	float DeltaTime = World->DeltaTimeSeconds;
	FVector ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce /** DeltaTime * 100.0f*/;
	FVector ForceLocation = GetComponentLocation();
	if (Throttle != 0.0f)
	{
		FString str = FString::Printf(TEXT("Force : %f, Location : %s"), ForceApplied.Size(), *ForceLocation.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, str);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Gravity : %f"), GetWorld()->GetGravityZ()));
		AddForceServer(Throttle);
	}
	
}

void UTankTrackStaticMeshComponent::AddForceServer_Implementation(float Throttle) {

	//设置力和位置 
	UWorld* World = GetWorld();
	check(World != nullptr);
	float DeltaTime = World->DeltaTimeSeconds;
	FVector ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce /** DeltaTime * 100.0f*/;
	FVector ForceLocation = GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Force : %f, Location : %s"), *Force.ToString());
	

	//获取根组件
	auto Owner = GetOwner();
	check(Owner != nullptr);
	auto TankRootComp = Cast<UPrimitiveComponent>(Owner->GetRootComponent());


	UE_LOG(LogTemp, Warning, TEXT("Force : %f, Location : %s"), ForceApplied.Size(), *ForceLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Gravity : %f"), GetWorld()->GetGravityZ());

	//check(GEngine != nullptr);
	//FString str = FString::Printf(TEXT("Force : %s"), *Force.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, str);
	//施加力
	TankRootComp->AddForceAtLocation(ForceApplied, ForceLocation);
}
