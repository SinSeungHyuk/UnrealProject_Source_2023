// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "PSGameInstance.generated.h"

/**
 * 
 */

class UDataTable;

USTRUCT(BlueprintType)
struct FPSCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPSCharacterData() : Level(1), Attack(50) , MaxHP(100.0f), Armor(0.f), Speed(0), Dodge(0), Luck(0), HPRegen(0), Lifesteal(0.f), AttackSpeed(0.f), NeedEXP(20) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Armor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Dodge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Luck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 HPRegen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Lifesteal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 NeedEXP;
};

USTRUCT(BlueprintType)
struct FPSEnemyData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPSEnemyData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString EnemyName;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float SetMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 SetAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float SetArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 SetSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 SetDodge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 SetLuck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 SetHPRegen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float SetLifesteal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float SetAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString ItemText1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString ItemText2;
};


UCLASS()
class PS_API UPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPSGameInstance();
	FPSCharacterData* GetPSCharacterData(int32 Level);
	FPSEnemyData* GetPSEnemyData(int32 Level);
	FItemData* GetItemData(int32 n);
	virtual void Init() override;

private:
	UPROPERTY()
		UDataTable* PSCharacterTable;
	UPROPERTY()
		UDataTable* PSEnemyTable;
	UPROPERTY()
		UDataTable* ItemTable;
};
