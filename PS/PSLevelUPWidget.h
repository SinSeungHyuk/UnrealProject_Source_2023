// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PSLevelUPWidget.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UPSLevelUPWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindCharacterStat(class UPSCharacterStatComponent* NewCharacterStat);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
		TArray<UTexture2D*> img;

	void SetOnBtnAction(UButton* btn, UTextBlock* txt, int32 idx, int32 level);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnBtnUpgradeClicked1();
	UFUNCTION()
		void OnBtnUpgradeClicked2();
	UFUNCTION()
		void OnBtnUpgradeClicked3();
	UFUNCTION()
		void OnBtnUpgradeClicked4();
	UFUNCTION()
		void OnBtnUpgradeClicked5();
	UFUNCTION()
		void OnBtnUpgradeClicked6();
	UFUNCTION()
		void OnBtnUpgradeClicked7();
	UFUNCTION()
		void OnBtnUpgradeClicked8();
	UFUNCTION()
		void OnBtnUpgradeClicked9();
	UFUNCTION()
		void OnBtnUpgradeClicked10();
	UFUNCTION()
		void OnBtnUpgradeClicked11();
	UFUNCTION()
		void OnBtnUpgradeClicked12();
	UFUNCTION()
		void OnBtnUpgradeClicked13();
	UFUNCTION()
		void OnBtnUpgradeClicked14();
	UFUNCTION()
		void OnBtnUpgradeClicked15();
	UFUNCTION()
		void OnBtnUpgradeClicked16();
	UFUNCTION()
		void OnBtnUpgradeClicked17();
	UFUNCTION()
		void OnBtnUpgradeClicked18();
	UFUNCTION()
		void OnBtnUpgradeClicked19();
	UFUNCTION()
		void OnBtnUpgradeClicked20();
	UFUNCTION()
		void OnBtnUpgradeClicked21();
	UFUNCTION()
		void OnBtnUpgradeClicked22();
	UFUNCTION()
		void OnBtnUpgradeClicked23();
	UFUNCTION()
		void OnBtnUpgradeClicked24();
	UFUNCTION()
		void OnBtnUpgradeClicked25();
	UFUNCTION()
		void OnBtnUpgradeClicked26();
	UFUNCTION()
		void OnBtnUpgradeClicked27();

private:
	TWeakObjectPtr<class UPSCharacterStatComponent> CurrentCharacterStat;
	TArray<int32> randImg;
	TArray<int32> randNum;

	UPROPERTY(meta=(BindWidget))
		class  UButton* BtnUpgrade_1;
	UPROPERTY(meta = (BindWidget))
		class  UButton* BtnUpgrade_2;
	UPROPERTY(meta = (BindWidget))
		class  UButton* BtnUpgrade_3;

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
		class UTextBlock* TxtStat_1;
	UPROPERTY()
		class UTextBlock* TxtStat_2;
	UPROPERTY()
		class UTextBlock* TxtStat_3;

	UPROPERTY()
		class UImage* ImgStat_1;
	UPROPERTY()
		class UImage* ImgStat_2;
	UPROPERTY()
		class UImage* ImgStat_3;
};
