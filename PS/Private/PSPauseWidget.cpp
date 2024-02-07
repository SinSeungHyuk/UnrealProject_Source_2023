// Fill out your copyright notice in the Description page of Project Settings.


#include "PSPauseWidget.h"
#include "PSCharacterStatComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPSPauseWidget::BindCharacterStat(UPSCharacterStatComponent* NewCharacterStat)
{
	if (nullptr != NewCharacterStat)
	{
		CurrentCharacterStat = NewCharacterStat;
	}
	else
		UE_LOG(LogTemp, Error, TEXT("character state widget error"));
}

void UPSPauseWidget::NativeConstruct()
{
	TxtMaxHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxHP")));
	TxtAttack = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtAttack")));
	TxtArmor = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtArmor")));
	TxtDodge = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtDodge")));
	TxtSpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtSpeed")));
	TxtHPRegen = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHPRegen")));
	TxtLifesteal = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLifesteal")));
	TxtAttackSpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtAttackSpeed")));
	TxtLuck = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLuck")));  // 현재 플레이어의 스탯 정보

	TxtMaxHP->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetHP())));
	TxtAttack->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetAttack())));
	TxtArmor->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetArmor() * 100.0f)));
	TxtDodge->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetDodge())));
	TxtSpeed->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetSpeed())));
	TxtHPRegen->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetHPRegen())));
	TxtLifesteal->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetLifesteal() * 1000.0f)));
	TxtAttackSpeed->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetAttackSpeed() * 100.0f)));
	TxtLuck->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetLuck())));

	UGameplayStatics::SetGamePaused(GetWorld(),true); // 게임 일시정지
}
