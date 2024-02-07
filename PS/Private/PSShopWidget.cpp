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
	ImgItem_1 = Cast<UImage>(GetWidgetFromName(TEXT("ImgItem_1")));  // 1�� ������

	BtnUpgrade_2 = Cast<UButton>(GetWidgetFromName(TEXT("BtnUpgrade_2")));
	TxtItemName_2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtItemName_2")));
	TxtStat_3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_3")));
	TxtStat_4 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_4")));
	TxtPrice_2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtPrice_2")));
	ImgItem_2 = Cast<UImage>(GetWidgetFromName(TEXT("ImgItem_2")));  // 2�� ������

	BtnUpgrade_3 = Cast<UButton>(GetWidgetFromName(TEXT("BtnUpgrade_3")));
	TxtItemName_3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtItemName_3")));
	TxtStat_5 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_5")));
	TxtStat_6 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtStat_6")));
	TxtPrice_3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtPrice_3")));
	ImgItem_3 = Cast<UImage>(GetWidgetFromName(TEXT("ImgItem_3")));  // 3�� ������

	TxtMaxHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMaxHP")));
	TxtAttack = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtAttack")));
	TxtArmor = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtArmor")));
	TxtDodge = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtDodge")));
	TxtSpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtSpeed")));
	TxtHPRegen = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHPRegen")));
	TxtLifesteal = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLifesteal")));
	TxtAttackSpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtAttackSpeed")));
	TxtLuck = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLuck"))); // ���� ĳ������ ��������

	CurrentGold = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtGold")));
	TxtReroll = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtReroll")));
	UpdateCurrentGold(); // �÷��̾��� ��差 ������Ʈ
	UpdateCurrentStat(); // �÷��̾��� ���� ���� ������Ʈ

	BtnReroll = Cast<UButton>(GetWidgetFromName(TEXT("BtnReroll")));
	BtnReroll->OnClicked.AddDynamic(this, &UPSShopWidget::Reroll); // ���� ��ư ��ɵ��

	BtnNextStage = Cast<UButton>(GetWidgetFromName(TEXT("BtnNextStage")));
	BtnNextStage->OnClicked.AddDynamic(this, &UPSShopWidget::NextStage); // ���� ��ư ��ɵ��

	SetItemList(); // ó�� UI ����� ������������ ��� �¾�
}

void UPSShopWidget::SetItemList() // ���������� ����� �������� �����ϴ� �Լ�
{
	for (int i = 0; i < 3; i++) {
		randImg.Insert(FMath::RandRange(0, 37), i); // �������� ���� ���� 
		for (int j = 0; j < i; j++) {
			if (randImg[i] == randImg[j]) i--;
		}
	}
	auto PSGameInstance = Cast<UPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CurrentItemData1 = PSGameInstance->GetItemData(randImg[0]);
	CurrentItemData2 = PSGameInstance->GetItemData(randImg[1]);
	CurrentItemData3 = PSGameInstance->GetItemData(randImg[2]); // ������ �����Ϳ��� ������ ��������

	TxtItemName_1->SetText(FText::FromString(CurrentItemData1->ItemName));
	TxtStat_1->SetText(FText::FromString(CurrentItemData1->ItemText1));
	TxtStat_2->SetText(FText::FromString(CurrentItemData1->ItemText2));
	TxtPrice_1->SetText(FText::FromString(FString::FromInt(CurrentItemData1->Gold)));
	ImgItem_1->SetBrushFromTexture(img[randImg[0]], true); // ������ �����Ϳ��� ����,����,�̸�,�̹��� ��������

	
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
	if (CurrentCharacterStat->GetGold() >= CurrentItemData1->Gold) { // ���� ��尡 ���ݺ��� �̻��� ���
		// ��带 �Ҹ��ϰ� ������ �����Ϳ� �ִ� ��ġ��ŭ ĳ���� ���� Set
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
		SetItemList(); // ���� �� �ٽ� ��������� ���ΰ�ħ
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
	if (CurrentCharacterStat->GetGold() >= RerollGold) { // ���� ��尡 ���Ѻ�� �̻��� ���
		SetItemList(); // ������ ��� ���ΰ�ħ
		CurrentCharacterStat->SetGold(-RerollGold); 
		RerollGold *= 1.1f; // ���� �Ҷ����� ������� ���� ���
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
		// �������� �������� ������ ������ ������ ����ǹǷ� �� ������ ���Ӱ� ������Ʈ
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