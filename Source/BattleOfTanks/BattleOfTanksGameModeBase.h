// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleOfTanksGameModeBase.generated.h"

class APlayerStart;

/**
 * 
 */
UCLASS()
class BATTLEOFTANKS_API ABattleOfTanksGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	


protected:

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	UPROPERTY(EditAnywhere, Category = "PlayerStart")
	TArray<AActor*> PlayerStarts;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerStart")
	TSubclassOf<APlayerStart> PlayerStartClass;

	int32 index;
};
