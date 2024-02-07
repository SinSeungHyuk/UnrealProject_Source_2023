// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSShopWidget.generated.h"

UCLASS()
class PS_API UPSShopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UPSCharacterStatComponent* NewCharacterStat);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
		TArray<UTexture2D*> img;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnBtnUpgradeClicked1();
	UFUNCTION()
		void OnBtnUpgradeClicked2();
	UFUNCTION()
		void OnBtnUpgradeClicked3();
	UFUNCTION()
		void Reroll();
	UFUNCTION()
		void NextStage();

	void SetItemList();
	void UpdateCurrentGold();
	void UpdateCurrentStat();

private:
	TArray<int32> randImg;

	UPROPERTY(meta = (BindWidget))
		class  UButton* BtnUpgrade_1;
	UPROPERTY(meta = (BindWidget))
		class  UButton* BtnUpgrade_2;
	UPROPERTY(meta = (BindWidget))
		class  UButton* BtnUpgrade_3;
	UPROPERTY(meta = (BindWidget))
		class  UButton* BtnReroll;
	UPROPERTY(meta = (BindWidget))
		class  UButton* BtnNextStage;

	UPROPERTY()
		class UTextBlock* TxtItemName_1;
	UPROPERTY()
		class UTextBlock* TxtItemName_2;
	UPROPERTY()
		class UTextBlock* TxtItemName_3;
	UPROPERTY()
		class UTextBlock* TxtStat_1;
	UPROPERTY()
		class UTextBlock* TxtStat_2;
	UPROPERTY()
		class UTextBlock* TxtStat_3;
	UPROPERTY()
		class UTextBlock* TxtStat_4;
	UPROPERTY()
		class UTextBlock* TxtStat_5;
	UPROPERTY()
		class UTextBlock* TxtStat_6;  // Item Name,Text

	UPROPERTY()
		class UTextBlock* TxtPrice_1;
	UPROPERTY()
		class UTextBlock* TxtPrice_2;
	UPROPERTY()
		class UTextBlock* TxtPrice_3; // Item Price

	UPROPERTY()
		class UImage* ImgItem_1;
	UPROPERTY()
		class UImage* ImgItem_2;
	UPROPERTY()
		class UImage* ImgItem_3; // Item Image

	UPROPERTY()
		class UTextBlock* TxtMaxHP;
	UPROPERTY()
		class UTextBlock* TxtArmor;
	UPROPERTY()
		class UTextBlock* TxtAttack;
	UPROPERTY()
		class UTextBlock* TxtDodge;
	UPROPERTY()
		class UTextBlock* TxtSpeed;
	UPROPERTY()
		class UTextBlock* TxtHPRegen;
	UPROPERTY()
		class UTextBlock* TxtLifesteal;
	UPROPERTY()
		class UTextBlock* TxtAttackSpeed;
	UPROPERTY()
		class UTextBlock* TxtLuck;
	UPROPERTY()
		class UTextBlock* CurrentGold;

	UPROPERTY()
		class UTextBlock* TxtReroll;

	TWeakObjectPtr<class UPSCharacterStatComponent> CurrentCharacterStat;

	struct FItemData* CurrentItemData1;
	struct FItemData* CurrentItemData2;
	struct FItemData* CurrentItemData3;

	int32 RerollGold;
};
