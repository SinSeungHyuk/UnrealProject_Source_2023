// Fill out your copyright notice in the Description page of Project Settings.


#include "PSEnemyStatComponent.h"
#include "PSGameInstance.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UPSEnemyStatComponent::UPSEnemyStatComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
	//Level = 1;
}

void UPSEnemyStatComponent::SetNewLevel(int32 newLevel) // 플레이어 레벨이 바뀔때마다 저장,레벨변경
{
	auto PSGameInstance = Cast<UPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CurrentStatData = PSGameInstance->GetPSEnemyData(newLevel);
	if (nullptr != CurrentStatData)
	{
		Level = newLevel;
		MaxHP = CurrentStatData->MaxHP;
		Attack = CurrentStatData->Attack;
		Speed = CurrentStatData->Speed;

		SetHP(CurrentStatData->MaxHP);
	}
}

void UPSEnemyStatComponent::SetHP(float newHP)
{
	CurrentHP = newHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

void UPSEnemyStatComponent::SetDamage(float Damage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - Damage, 0.0f, CurrentStatData->MaxHP));
}

float UPSEnemyStatComponent::getHPRatio()
{
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}