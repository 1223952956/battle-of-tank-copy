// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleOfTanksGameModeBase.generated.h"

class APlayerStart;
class UBattleOfTanksGameInstance;

/**
 * 
 */
UCLASS()
class BATTLEOFTANKS_API ABattleOfTanksGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:


protected:

	//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	//virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	//UPROPERTY(EditAnywhere, Category = "PlayerStart")
	//TArray<AActor*> PlayerStarts;

	//UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//TSubclassOf<APlayerStart> PlayerStartClass;

	//UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//TSubclassOf<UBattleOfTanksGameInstance> GameInstanceClass;

	//TArray<APlayerController*> PlayerControllers;

	//int32 index;
};
