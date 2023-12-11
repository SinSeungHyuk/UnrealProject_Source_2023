// Fill out your copyright notice in the Description page of Project Settings.


#include "PSEnemyHPBarWidget.h"
#include "PSEnemyStatComponent.h"
#include "Components/ProgressBar.h"

void UPSEnemyHPBarWidget::BindCharacterStat(UPSEnemyStatComponent* NewCharacterStat)
{
	if (nullptr != NewCharacterStat)
	{
		CurrentCharacterStat = NewCharacterStat;
		NewCharacterStat->OnHPChanged.AddUObject(this, &UPSEnemyHPBarWidget::UpdateHPWidget);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("character state widget error"));
}

void UPSEnemyHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("EnemyHPBar")));
	UpdateHPWidget();
}

void UPSEnemyHPBarWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->getHPRatio());
		}
	}
}
