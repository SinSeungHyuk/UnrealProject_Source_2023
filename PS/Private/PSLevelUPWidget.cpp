// Fill out your copyright notice in the Description page of Project Settings.

#include <random>
#include "Components/Image.h"
#include "PSLevelUPWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PSCharacterStatComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


void UPSLevelUPWidget::BindCharacterStat(UPSCharacterStatComponent* NewCharacterStat)
{
	if (nullptr != NewCharacterStat)
	{
		CurrentCharacterStat = NewCharacterStat;
	}
	else
		UE_LOG(LogTemp, Error, TEXT("character state widget error"));
}

void UPSLevelUPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BtnUpgrade_1 = Cast<UButton>(GetWidgetFromName(TEXT("BtnUpgrade_1")));
	BtnUpgrade_2 = Cast<UButton>(GetWidgetFromName(TEXT("BtnUpgrade_2")));
	BtnUpgrade_3 = Cast<UButton>(GetWidgetFromName(TEXT("BtnUpgrade_3"))); // 레벨업 버튼 3개

	TxtMaxHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxHP")));
	TxtAttack = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtAttack")));
	TxtArmor = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtArmor")));
	TxtDodge = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtDodge")));
	TxtSpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtSpeed")));
	TxtHPRegen = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHPRegen")));
	TxtLifesteal = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLifesteal")));
	TxtAttackSpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtAttackSpeed")));
	TxtLuck = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLuck")));  // 현재 플레이어의 스탯 정보

	TxtStat_1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_1")));
	TxtStat_2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_2")));
	TxtStat_3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_3"))); // 선택지 3개의 레벨업 효과

	ImgStat_1 = Cast<UImage>(GetWidgetFromName(TEXT("ImgStat_1")));
	ImgStat_2 = Cast<UImage>(GetWidgetFromName(TEXT("ImgStat_2")));
	ImgStat_3 = Cast<UImage>(GetWidgetFromName(TEXT("ImgStat_3"))); // 선택지 3개의 이미지

	TxtMaxHP->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetHP())));
	TxtAttack->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetAttack())));
	TxtArmor->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetArmor()*100.0f)));
	TxtDodge->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetDodge())));
	TxtSpeed->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetSpeed())));
	TxtHPRegen->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetHPRegen())));
	TxtLifesteal->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetLifesteal() * 1000.0f)));
	TxtAttackSpeed->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetAttackSpeed() * 100.0f)));
	TxtLuck->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetLuck())));

	for (int i = 0; i < 3; i++) {
		randImg.Insert(FMath::RandRange(0, 8),i);
		for (int j = 0; j < i; j++) {
			if (randImg[i] == randImg[j]) i--; // 9개의 스탯중 3개 랜덤으로 결정 (이미지 번호)
		}
	}

	ImgStat_1->SetBrushFromTexture(img[randImg[0]], true);
	ImgStat_2->SetBrushFromTexture(img[randImg[1]], true);
	ImgStat_3->SetBrushFromTexture(img[randImg[2]], true); // 스탯의 이미지 3개 Set

	for (int i = 0; i < 3; i++) {
		randNum.Insert(FMath::RandRange(1,10), i); // 각 선택지별 확률에 따른 등급을 위한 난수 생성
	}

	SetOnBtnAction(BtnUpgrade_1, TxtStat_1, randImg[0], randNum[0]);
	SetOnBtnAction(BtnUpgrade_2, TxtStat_2, randImg[1], randNum[1]);
	SetOnBtnAction(BtnUpgrade_3, TxtStat_3, randImg[2], randNum[2]); // 각 선택지별 버튼기능,텍스트 설정 함수
}

void UPSLevelUPWidget::SetOnBtnAction(UButton* btn, UTextBlock* txt, int32 idx, int32 level)
{
	switch (idx) // 스위치 변수로 어떤 스탯인지 종류가 담겨있는 randImg를 받음
	{
	case 0:
		if (level < 8) { // 1~7, 70%의 확률로 나오는 1단계 레벨업 효과
			txt->SetText(FText::FromString("MaxHP +3"));
			txt->SetColorAndOpacity(FLinearColor::White);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked1);
		}
		else if (level < 10 && level > 7) { // 8~9, 20%의 확률로 나오는 2단계 레벨업 효과
			txt->SetText(FText::FromString("MaxHP +5"));
			txt->SetColorAndOpacity(FLinearColor::Green);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked2);
		}
		else { // 10, 10%의 확률로 나오는 3단계 레벨업 효과
			txt->SetText(FText::FromString("MaxHP +7"));
			txt->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 1.0f, 1.0f));
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked3);
		}
		break;
	case 1:
		if (level < 8) {
			txt->SetText(FText::FromString("Attack +3"));
			txt->SetColorAndOpacity(FLinearColor::White);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked4);
		}
		else if (level < 10 && level > 7) {
			txt->SetText(FText::FromString("Attack +5"));
			txt->SetColorAndOpacity(FLinearColor::Green);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked5);
		}
		else {
			txt->SetText(FText::FromString("Attack +7"));
			txt->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 1.0f, 1.0f));
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked6);
		}
		break;
	case 2:
		if (level < 8) {
			txt->SetText(FText::FromString("Armor +3"));
			txt->SetColorAndOpacity(FLinearColor::White);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked7);
		}
		else if (level < 10 && level > 7) {
			txt->SetText(FText::FromString("Armor +4"));
			txt->SetColorAndOpacity(FLinearColor::Green);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked8);
		}
		else {
			txt->SetText(FText::FromString("Armor +5"));
			txt->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 1.0f, 1.0f));
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked9);
		}
		break;
	case 3:
		if (level < 8) {
			txt->SetText(FText::FromString("Speed +7"));
			txt->SetColorAndOpacity(FLinearColor::White);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked10);
		}
		else if (level < 10 && level > 7) {
			txt->SetText(FText::FromString("Speed +10"));
			txt->SetColorAndOpacity(FLinearColor::Green);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked11);
		}
		else {
			txt->SetText(FText::FromString("Speed +15"));
			txt->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 1.0f, 1.0f));
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked12);
		}
		break;
	case 4:
		if (level < 8) {
			txt->SetText(FText::FromString("Dodge +3"));
			txt->SetColorAndOpacity(FLinearColor::White);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked13);
		}
		else if (level < 10 && level > 7) {
			txt->SetText(FText::FromString("Dodge +4"));
			txt->SetColorAndOpacity(FLinearColor::Green);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked14);
		}
		else {
			txt->SetText(FText::FromString("Dodge +5"));
			txt->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 1.0f, 1.0f));
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked15);
		}
		break;
	case 5:
		if (level < 8) {
			txt->SetText(FText::FromString("Luck +1"));
			txt->SetColorAndOpacity(FLinearColor::White);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked16);
		}
		else if (level < 10 && level > 7) {
			txt->SetText(FText::FromString("Luck +2"));
			txt->SetColorAndOpacity(FLinearColor::Green);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked17);
		}
		else {
			txt->SetText(FText::FromString("Luck +3"));
			txt->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 1.0f, 1.0f));
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked18);
		}
		break;
	case 6:
		if (level < 8) {
			txt->SetText(FText::FromString("HP Regen +1"));
			txt->SetColorAndOpacity(FLinearColor::White);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked19);
		}
		else if (level < 10 && level > 7) {
			txt->SetText(FText::FromString("HP Regen +2"));
			txt->SetColorAndOpacity(FLinearColor::Green);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked20);
		}
		else {
			txt->SetText(FText::FromString("HP Regen +3"));
			txt->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 1.0f, 1.0f));
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked21);
		}
		break;
	case 7:
		if (level < 8) {
			txt->SetText(FText::FromString("Lifesteal +1"));
			txt->SetColorAndOpacity(FLinearColor::White);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked22);
		}
		else if (level < 10 && level > 7) {
			txt->SetText(FText::FromString("Lifesteal +2"));
			txt->SetColorAndOpacity(FLinearColor::Green);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked23);
		}
		else {
			txt->SetText(FText::FromString("Lifesteal +4"));
			txt->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 1.0f, 1.0f));
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked24);
		}
		break;
	case 8:
		if (level < 8) {
			txt->SetText(FText::FromString("AttackSpeed +5"));
			txt->SetColorAndOpacity(FLinearColor::White);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked25);
		}
		else if (level < 10 && level > 7) {
			txt->SetText(FText::FromString("AttackSpeed +8"));
			txt->SetColorAndOpacity(FLinearColor::Green);
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked26);
		}
		else {
			txt->SetText(FText::FromString("AttackSpeed +10"));
			txt->SetColorAndOpacity(FLinearColor(1.0f,0.0f,1.0f,1.0f));
			btn->OnClicked.AddDynamic(this, &UPSLevelUPWidget::OnBtnUpgradeClicked27);
		}
		break;
	}
}


// ------------------------------------------------------SetMaxHP
void UPSLevelUPWidget::OnBtnUpgradeClicked1()
{
	CurrentCharacterStat->SetMaxHP(3.0f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked2()
{
	CurrentCharacterStat->SetMaxHP(5.0f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked3()
{
	CurrentCharacterStat->SetMaxHP(7.0f);
}
// ------------------------------------------------------SetAttack
void UPSLevelUPWidget::OnBtnUpgradeClicked4()
{
	CurrentCharacterStat->SetAttack(3.0f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked5()
{
	CurrentCharacterStat->SetAttack(5.0f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked6()
{
	CurrentCharacterStat->SetAttack(7.0f);
}
// ------------------------------------------------------SetArmor
void UPSLevelUPWidget::OnBtnUpgradeClicked7()
{
	CurrentCharacterStat->SetArmor(0.03f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked8()
{
	CurrentCharacterStat->SetArmor(0.04f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked9()
{
	CurrentCharacterStat->SetArmor(0.05f);
}
// ------------------------------------------------------SetSpeed
void UPSLevelUPWidget::OnBtnUpgradeClicked10()
{
	CurrentCharacterStat->SetSpeed(7);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked11()
{
	CurrentCharacterStat->SetSpeed(10);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked12()
{
	CurrentCharacterStat->SetSpeed(15);
}
// ------------------------------------------------------SetDodge
void UPSLevelUPWidget::OnBtnUpgradeClicked13()
{
	CurrentCharacterStat->SetDodge(3);
}
void UPSLevelUPWidget::OnBtnUpgradeClicked14()
{
	CurrentCharacterStat->SetDodge(4);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked15()
{
	CurrentCharacterStat->SetDodge(5);
}
// ------------------------------------------------------SetLuck
void UPSLevelUPWidget::OnBtnUpgradeClicked16()
{
	CurrentCharacterStat->SetLuck(1);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked17()
{
	CurrentCharacterStat->SetLuck(2);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked18()
{
	CurrentCharacterStat->SetLuck(3);
}
// ------------------------------------------------------SetHPRegen
void UPSLevelUPWidget::OnBtnUpgradeClicked19()
{
	CurrentCharacterStat->SetHPRegen(1.0f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked20()
{
	CurrentCharacterStat->SetHPRegen(2.0f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked21()
{
	CurrentCharacterStat->SetHPRegen(3.0f);
}
// ------------------------------------------------------SetLifesteal
void UPSLevelUPWidget::OnBtnUpgradeClicked22()
{
	CurrentCharacterStat->SetLifesteal(0.001f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked23()
{
	CurrentCharacterStat->SetLifesteal(0.002f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked24()
{
	CurrentCharacterStat->SetLifesteal(0.004f);
}
// ------------------------------------------------------AttackSpeed
void UPSLevelUPWidget::OnBtnUpgradeClicked25()
{
	CurrentCharacterStat->SetAttackSpeed(0.05f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked26()
{
	CurrentCharacterStat->SetAttackSpeed(0.08f);
}

void UPSLevelUPWidget::OnBtnUpgradeClicked27()
{
	CurrentCharacterStat->SetAttackSpeed(0.10f);
}