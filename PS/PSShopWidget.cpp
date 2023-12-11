// Fill out your copyright notice in the Description page of Project Settings.


#include "PSShopWidget.h"
#include "Components/TextBlock.h"
#include <random>
#include "PSGameInstance.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PSCharacterStatComponent.h"

void UPSShopWidget::BindCharacterStat(UPSCharacterStatComponent* NewCharacterStat)
{
	if (nullptr != NewCharacterStat)
	{
		CurrentCharacterStat = NewCharacterStat;
		NewCharacterStat->OnGoldChanged.AddUObject(this, &UPSShopWidget::UpdateCurrentGold);
		NewCharacterStat->OnAttackChanged.AddUObject(this, &UPSShopWidget::UpdateCurrentStat);
		NewCharacterStat->OnMaxMPChanged.AddUObject(this, &UPSShopWidget::UpdateCurrentStat);
		NewCharacterStat->OnArmorChanged.AddUObject(this, &UPSShopWidget::UpdateCurrentStat);
		NewCharacterStat->OnSpeedChanged.AddUObject(this, &UPSShopWidget::UpdateCurrentStat);
		NewCharacterStat->OnDodgeChanged.AddUObject(this, &UPSShopWidget::UpdateCurrentStat);
		NewCharacterStat->OnLuckChanged.AddUObject(this, &UPSShopWidget::UpdateCurrentStat);
		NewCharacterStat->OnHPRegenChanged.AddUObject(this, &UPSShopWidget::UpdateCurrentStat);
		NewCharacterStat->OnLifestealChanged.AddUObject(this, &UPSShopWidget::UpdateCurrentStat);
		NewCharacterStat->OnAttackSpeedChanged.AddUObject(this, &UPSShopWidget::UpdateCurrentStat);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("character state widget error"));
}

void UPSShopWidget::NativeConstruct() {
	Super::NativeConstruct();

	RerollGold = 20;

	BtnUpgrade_1 = Cast<UButton>(GetWidgetFromName(TEXT("BtnUpgrade_1")));
	BtnUpgrade_1->OnClicked.AddDynamic(this, &UPSShopWidget::OnBtnUpgradeClicked1);
	BtnUpgrade_2 = Cast<UButton>(GetWidgetFromName(TEXT("BtnUpgrade_2")));
	BtnUpgrade_2->OnClicked.AddDynamic(this, &UPSShopWidget::OnBtnUpgradeClicked2);
	BtnUpgrade_3 = Cast<UButton>(GetWidgetFromName(TEXT("BtnUpgrade_3")));
	BtnUpgrade_3->OnClicked.AddDynamic(this, &UPSShopWidget::OnBtnUpgradeClicked3);

	TxtItemName_1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtItemName_1")));
	TxtStat_1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_1")));
	TxtStat_2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_2")));
	TxtPrice_1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtPrice_1")));
	ImgItem_1 = Cast<UImage>(GetWidgetFromName(TEXT("ImgItem_1")));  // 1번 아이템

	BtnUpgrade_2 = Cast<UButton>(GetWidgetFromName(TEXT("BtnUpgrade_2")));
	TxtItemName_2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtItemName_2")));
	TxtStat_3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_3")));
	TxtStat_4 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_4")));
	TxtPrice_2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtPrice_2")));
	ImgItem_2 = Cast<UImage>(GetWidgetFromName(TEXT("ImgItem_2")));  // 2번 아이템

	BtnUpgrade_3 = Cast<UButton>(GetWidgetFromName(TEXT("BtnUpgrade_3")));
	TxtItemName_3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtItemName_3")));
	TxtStat_5 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_5")));
	TxtStat_6 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_6")));
	TxtPrice_3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtPrice_3")));
	ImgItem_3 = Cast<UImage>(GetWidgetFromName(TEXT("ImgItem_3")));  // 3번 아이템

	TxtMaxHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxHP")));
	TxtAttack = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtAttack")));
	TxtArmor = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtArmor")));
	TxtDodge = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtDodge")));
	TxtSpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtSpeed")));
	TxtHPRegen = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHPRegen")));
	TxtLifesteal = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLifesteal")));
	TxtAttackSpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtAttackSpeed")));
	TxtLuck = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLuck"))); // 현재 캐릭터의 스탯정보

	CurrentGold = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtGold")));
	TxtReroll = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtReroll")));
	UpdateCurrentGold(); // 플레이어의 골드량 업데이트
	UpdateCurrentStat(); // 플레이어의 스탯 정보 업데이트

	BtnReroll = Cast<UButton>(GetWidgetFromName(TEXT("BtnReroll")));
	BtnReroll->OnClicked.AddDynamic(this, &UPSShopWidget::Reroll); // 리롤 버튼 기능등록

	BtnNextStage = Cast<UButton>(GetWidgetFromName(TEXT("BtnNextStage")));
	BtnNextStage->OnClicked.AddDynamic(this, &UPSShopWidget::NextStage); // 리롤 버튼 기능등록

	SetItemList(); // 처음 UI 실행시 상점아이템의 목록 셋업
}

void UPSShopWidget::SetItemList() // 상점아이템 목록을 랜덤으로 세팅하는 함수
{
	for (int i = 0; i < 3; i++) {
		randImg.Insert(FMath::RandRange(0, 37), i); // 아이템의 종류 결정 
		for (int j = 0; j < i; j++) {
			if (randImg[i] == randImg[j]) i--;
		}
	}
	auto PSGameInstance = Cast<UPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CurrentItemData1 = PSGameInstance->GetItemData(randImg[0]);
	CurrentItemData2 = PSGameInstance->GetItemData(randImg[1]);
	CurrentItemData3 = PSGameInstance->GetItemData(randImg[2]); // 아이템 데이터에서 데이터 가져오기

	TxtItemName_1->SetText(FText::FromString(CurrentItemData1->ItemName));
	TxtStat_1->SetText(FText::FromString(CurrentItemData1->ItemText1));
	TxtStat_2->SetText(FText::FromString(CurrentItemData1->ItemText2));
	TxtPrice_1->SetText(FText::FromString(FString::FromInt(CurrentItemData1->Gold)));
	ImgItem_1->SetBrushFromTexture(img[randImg[0]], true); // 아이템 데이터에서 가격,설명,이름,이미지 가져오기

	
	TxtItemName_2->SetText(FText::FromString(CurrentItemData2->ItemName));
	TxtStat_3->SetText(FText::FromString(CurrentItemData2->ItemText1));
	TxtStat_4->SetText(FText::FromString(CurrentItemData2->ItemText2));
	TxtPrice_2->SetText(FText::FromString(FString::FromInt(CurrentItemData2->Gold)));
	ImgItem_2->SetBrushFromTexture(img[randImg[1]], true);


	TxtItemName_3->SetText(FText::FromString(CurrentItemData3->ItemName));
	TxtStat_5->SetText(FText::FromString(CurrentItemData3->ItemText1));
	TxtStat_6->SetText(FText::FromString(CurrentItemData3->ItemText2));
	TxtPrice_3->SetText(FText::FromString(FString::FromInt(CurrentItemData3->Gold)));
	ImgItem_3->SetBrushFromTexture(img[randImg[2]], true);

}

void UPSShopWidget::OnBtnUpgradeClicked1()
{
	if (CurrentCharacterStat->GetGold() >= CurrentItemData1->Gold) { // 보유 골드가 가격보다 이상일 경우
		// 골드를 소모하고 아이템 데이터에 있는 수치만큼 캐릭터 스탯 Set
		CurrentCharacterStat->SetGold(-CurrentItemData1->Gold);	
		CurrentCharacterStat->SetMaxHP(CurrentItemData1->SetMaxHP);
		CurrentCharacterStat->SetAttack(CurrentItemData1->SetAttack);
		CurrentCharacterStat->SetArmor(CurrentItemData1->SetArmor);
		CurrentCharacterStat->SetSpeed(CurrentItemData1->SetSpeed);
		CurrentCharacterStat->SetDodge(CurrentItemData1->SetDodge);
		CurrentCharacterStat->SetLuck(CurrentItemData1->SetLuck);
		CurrentCharacterStat->SetHPRegen(CurrentItemData1->SetHPRegen);
		CurrentCharacterStat->SetLifesteal(CurrentItemData1->SetLifesteal);
		CurrentCharacterStat->SetAttackSpeed(CurrentItemData1->SetAttackSpeed);
		SetItemList(); // 구입 후 다시 상점목록을 새로고침
	}
}
void UPSShopWidget::OnBtnUpgradeClicked2()
{
	if (CurrentCharacterStat->GetGold() >= CurrentItemData2->Gold) {
		CurrentCharacterStat->SetGold(-CurrentItemData2->Gold);
		CurrentCharacterStat->SetMaxHP(CurrentItemData2->SetMaxHP);
		CurrentCharacterStat->SetAttack(CurrentItemData2->SetAttack);	
		CurrentCharacterStat->SetArmor(CurrentItemData2->SetArmor);
		CurrentCharacterStat->SetSpeed(CurrentItemData2->SetSpeed);
		CurrentCharacterStat->SetDodge(CurrentItemData2->SetDodge);
		CurrentCharacterStat->SetLuck(CurrentItemData2->SetLuck);
		CurrentCharacterStat->SetHPRegen(CurrentItemData2->SetHPRegen);
		CurrentCharacterStat->SetLifesteal(CurrentItemData2->SetLifesteal);
		CurrentCharacterStat->SetAttackSpeed(CurrentItemData2->SetAttackSpeed);
		SetItemList();
	}
}
void UPSShopWidget::OnBtnUpgradeClicked3()
{
	if (CurrentCharacterStat->GetGold() >= CurrentItemData3->Gold) {
		CurrentCharacterStat->SetGold(-CurrentItemData3->Gold);
		CurrentCharacterStat->SetMaxHP(CurrentItemData3->SetMaxHP);
		CurrentCharacterStat->SetAttack(CurrentItemData3->SetAttack);
		CurrentCharacterStat->SetArmor(CurrentItemData3->SetArmor);
		CurrentCharacterStat->SetSpeed(CurrentItemData3->SetSpeed);
		CurrentCharacterStat->SetDodge(CurrentItemData3->SetDodge);
		CurrentCharacterStat->SetLuck(CurrentItemData3->SetLuck);
		CurrentCharacterStat->SetHPRegen(CurrentItemData3->SetHPRegen);
		CurrentCharacterStat->SetLifesteal(CurrentItemData3->SetLifesteal);
		CurrentCharacterStat->SetAttackSpeed(CurrentItemData3->SetAttackSpeed);
		SetItemList();
	}
}

void UPSShopWidget::Reroll()
{
	if (CurrentCharacterStat->GetGold() >= RerollGold) { // 보유 골드가 리롤비용 이상일 경우
		SetItemList(); // 아이템 목록 새로고침
		CurrentCharacterStat->SetGold(-RerollGold); 
		RerollGold *= 1.1f; // 리롤 할때마다 비싸지는 리롤 비용
		TxtReroll->SetText(FText::FromString(FString::FromInt(RerollGold)));
	}
}

void UPSShopWidget::NextStage()
{
	CurrentCharacterStat->Save();
}

void UPSShopWidget::UpdateCurrentGold()
{
	if (CurrentCharacterStat.IsValid())
	{
		CurrentGold->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
	}
}

void UPSShopWidget::UpdateCurrentStat()
{
	if (CurrentCharacterStat.IsValid())
	{
		// 상점에서 아이템을 구입할 때마다 스탯이 변경되므로 각 스탯을 새롭게 업데이트
		TxtMaxHP->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetHP())));
		TxtAttack->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetAttack())));
		TxtArmor->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetArmor() * 100.0f)));
		TxtDodge->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetDodge())));
		TxtSpeed->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetSpeed())));
		TxtHPRegen->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetHPRegen())));
		TxtLifesteal->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetLifesteal() * 1000.0f)));
		TxtAttackSpeed->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetAttackSpeed() * 100.0f)));
		TxtLuck->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetLuck())));
	}
}