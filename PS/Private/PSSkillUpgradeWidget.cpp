// Fill out your copyright notice in the Description page of Project Settings.


#include "PSSkillUpgradeWidget.h"
#include "Components/TextBlock.h"
#include "PSGameInstance.h"
#include "Components/Image.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PSCharacterStatComponent.h"


void UPSSkillUpgradeWidget::BindCharacterStat(int32 Upgrade)
{
	UpgradeMode = Upgrade;
}

void UPSSkillUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TxtUpgradeTitle = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtUpgradeTitle")));
	TxtUpgrade = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtUpgrade")));
	ImgStat = Cast<UImage>(GetWidgetFromName(TEXT("ImgStat")));

	switch (UpgradeMode)
	{
	case 1:
		TxtUpgradeTitle->SetText(FText::FromString("MaxHP Upgrade"));
		TxtUpgrade->SetText(FText::FromString("Damage increases\nproportional to 'MaxHP'"));
		ImgStat->SetBrushFromTexture(img[0]);
		break;
	case 2:
		TxtUpgradeTitle->SetText(FText::FromString("Attack Upgrade"));
		TxtUpgrade->SetText(FText::FromString("Damage increases\nproportional to 'Attack'"));
		ImgStat->SetBrushFromTexture(img[1]);
		break;
	case 3:
		TxtUpgradeTitle->SetText(FText::FromString("Armor Upgrade"));
		TxtUpgrade->SetText(FText::FromString("Damage increases\nproportional to 'Armor'"));
		ImgStat->SetBrushFromTexture(img[2]);
		break;
	case 4:
		TxtUpgradeTitle->SetText(FText::FromString("Speed Upgrade"));
		TxtUpgrade->SetText(FText::FromString("Damage increases\nproportional to 'Speed'"));
		ImgStat->SetBrushFromTexture(img[3]);
		break;
	case 5:
		TxtUpgradeTitle->SetText(FText::FromString("Dodge Upgrade"));
		TxtUpgrade->SetText(FText::FromString("Damage increases\nproportional to 'Dodge'"));
		ImgStat->SetBrushFromTexture(img[4]);
		break;
	case 6:
		TxtUpgradeTitle->SetText(FText::FromString("Luck Upgrade"));
		TxtUpgrade->SetText(FText::FromString("Damage increases\nproportional to 'Luck'"));
		ImgStat->SetBrushFromTexture(img[5]);
		break;
	case 7:
		TxtUpgradeTitle->SetText(FText::FromString("HPRegen Upgrade"));
		TxtUpgrade->SetText(FText::FromString("Damage increases\nproportional to 'HPRegen'"));
		ImgStat->SetBrushFromTexture(img[6]);
		break;
	case 8:
		TxtUpgradeTitle->SetText(FText::FromString("Lifesteal Upgrade"));
		TxtUpgrade->SetText(FText::FromString("Damage increases\nproportional to 'Lifesteal'"));
		ImgStat->SetBrushFromTexture(img[7]);
		break;
	case 9:
		TxtUpgradeTitle->SetText(FText::FromString("AttackSpeed Upgrade"));
		TxtUpgrade->SetText(FText::FromString("Damage increases\nproportional to 'AttackSpeed'"));
		ImgStat->SetBrushFromTexture(img[8]);
		break;
	default:
		break;
	}
}
