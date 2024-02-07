// Fill out your copyright notice in the Description page of Project Settings.


#include "PSGameInstance.h"

UPSGameInstance::UPSGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Data/PSCharacterData.PSCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PS(*CharacterDataPath);
	if (DT_PS.Succeeded()) {
		PSCharacterTable = DT_PS.Object;
	}

	FString EnemyDataPath = TEXT("/Game/Data/PSEnemyData.PSEnemyData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PSE(*EnemyDataPath);
	if (DT_PSE.Succeeded()) {
		PSEnemyTable = DT_PSE.Object;
	}

	FString ItemDataPath = TEXT("/Game/Data/ItemData.ItemData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEM(*ItemDataPath);
	if (DT_ITEM.Succeeded()) {
		ItemTable = DT_ITEM.Object;
	}
}


FPSCharacterData* UPSGameInstance::GetPSCharacterData(int32 Level)
{
	return PSCharacterTable->FindRow<FPSCharacterData>(*FString::FromInt(Level), TEXT(""));
}

FPSEnemyData* UPSGameInstance::GetPSEnemyData(int32 Level)
{
	return PSEnemyTable->FindRow<FPSEnemyData>(*FString::FromInt(Level), TEXT(""));
}

FItemData* UPSGameInstance::GetItemData(int32 n)
{
	return ItemTable->FindRow<FItemData>(*FString::FromInt(n), TEXT(""));
}

void UPSGameInstance::Init()
{
	Super::Init();
}