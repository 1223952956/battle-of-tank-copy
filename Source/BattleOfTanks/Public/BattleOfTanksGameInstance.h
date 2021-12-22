// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleOfTanksGameInstance.generated.h"


//USTRUCT(BlueprintType)
//struct FPlayerInfo
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Info")
//	FString PlayerName;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Info")
//	FString PlayerRole;
//
//
//};
//
//UENUM(BlueprintType)
//enum class ERoomType : uint8
//{
//	ERoom_Team        UMETA(DisplayName = "TeamPlay"),
//	ERoom_Melee       UMETA(DisplayName = "MeleePlay"),
//};
//
//USTRUCT(BlueprintType)
//struct FRoomInfo
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Info")
//	int32 MaxPlayerNum;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "info")
//	ERoomType RoomType;
//};


/**
 * 
 */
UCLASS()
class BATTLEOFTANKS_API UBattleOfTanksGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
//public:
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
//	FPlayerInfo PlayerInfo;
//
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
//	FRoomInfo RoomInfo;
};


