// Copyright Epic Games, Inc. All Rights Reserved.


#include "BattleOfTanksGameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "BattleOfTanksGameInstance.h"


//AActor* ABattleOfTanksGameModeBase::ChoosePlayerStart_Implementation(AController* Player) {
//	if (PlayerStarts.Num() == 0) {
//		UWorld* World = GetWorld();
//		check(World != nullptr);
//
//		PlayerStartClass = APlayerStart::StaticClass();
//		//UE_LOG(LogTemp, Warning, TEXT("ABattleOfTanksGameModeBase::StartPlay()"));
//		UGameplayStatics::GetAllActorsOfClass(World, PlayerStartClass, PlayerStarts);
//	}
//	UE_LOG(LogTemp, Warning, TEXT("ABattleOfTanksGameModeBase::ChoosePlayerStart_Implementation()"));
//	if (PlayerStarts.Num() <= index) return nullptr;
//	return PlayerStarts[index++];
//}
//
//void ABattleOfTanksGameModeBase::PostLogin(APlayerController* NewPlayer) {
//	Super::PostLogin(NewPlayer);
//
//	PlayerControllers.Add(NewPlayer);
//}