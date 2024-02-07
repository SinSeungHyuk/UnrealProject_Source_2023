// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UPSSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float SaveAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float SaveMaxHP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float SaveArmor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveDodge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveLuck;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveHPRegen;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float SaveLifesteal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float SaveAttackSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		float SaveCurrentEXP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveGold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveUpgrageMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveEnemyKills;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		FString SaveStageName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		FString SaveSlotName;		// Save game data file name

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
		int32 SaveIndex;		// Save game data file index
};
