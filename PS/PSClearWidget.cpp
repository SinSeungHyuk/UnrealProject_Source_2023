// Fill out your copyright notice in the Description page of Project Settings.


#include "PSClearWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "PSCharacterStatComponent.h"
#include "PSCharacter.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Kismet/GameplayStatics.h"

void UPSClearWidget::BindCharacterStat(class UPSCharacterStatComponent* NewCharacterStat)
{
	if (nullptr != NewCharacterStat)
	{
		CurrentCharacterStat = NewCharacterStat;
	}
	else
		UE_LOG(LogTemp, Error, TEXT("character state widget error"));
}

void UPSClearWidget::NativeConstruct()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	BtnNextStage = Cast<UButton>(GetWidgetFromName(TEXT("BtnNextStage")));
	BtnNextStage->OnClicked.AddDynamic(this, &UPSClearWidget::ChangeLevel);

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

	TxtStat_1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_1")));
	TxtStat_2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_2")));
	TxtStat_3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_3"))); 

	ImgStat_1 = Cast<UImage>(GetWidgetFromName(TEXT("ImgStat_1")));

	switch (CurrentCharacterStat->GetUpgradeMode())
	{
	case 0:
		TxtStat_1->SetText(FText::FromString("Upgrade Mode : None"));
		ImgStat_1->SetBrushFromTexture(img[9], true);
		break;
	case 1:
		TxtStat_1->SetText(FText::FromString("Upgrade Mode : 'MaxHP'"));
		ImgStat_1->SetBrushFromTexture(img[0], true);
		break;
	case 2:
		TxtStat_1->SetText(FText::FromString("Upgrade Mode : 'Attack'"));
		ImgStat_1->SetBrushFromTexture(img[1], true);
		break;
	case 3:
		TxtStat_1->SetText(FText::FromString("Upgrade Mode : 'Armor'"));
		ImgStat_1->SetBrushFromTexture(img[2], true);
		break;
	case 4:
		TxtStat_1->SetText(FText::FromString("Upgrade Mode : 'Speed'"));
		ImgStat_1->SetBrushFromTexture(img[3], true);
		break;
	case 5:
		TxtStat_1->SetText(FText::FromString("Upgrade Mode : 'Dodge'"));
		ImgStat_1->SetBrushFromTexture(img[4], true);
		break;
	case 6:
		TxtStat_1->SetText(FText::FromString("Upgrade Mode : 'Luck'"));
		ImgStat_1->SetBrushFromTexture(img[5], true);
		break;
	case 7:
		TxtStat_1->SetText(FText::FromString("Upgrade Mode : 'HPRegen'"));
		ImgStat_1->SetBrushFromTexture(img[6], true);
		break;
	case 8:
		TxtStat_1->SetText(FText::FromString("Upgrade Mode : 'Lifesteal'"));
		ImgStat_1->SetBrushFromTexture(img[7], true);
		break;
	case 9:
		TxtStat_1->SetText(FText::FromString("Upgrade Mode : 'AttackSpeed'"));
		ImgStat_1->SetBrushFromTexture(img[8], true);
		break;
	default:
		break;
	}

	TxtStat_2->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->Level+1 ))); // 레벨
	TxtStat_3->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->EnemyKills ))); // 총 킬수

	APSCharacter* CharacterSt = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	CharacterSt->CharacterStat->DeleteSave(); // 클리어,실패 화면이 나타날 경우 세이브 삭제
}

void UPSClearWidget::ChangeLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainLevel"));
}
