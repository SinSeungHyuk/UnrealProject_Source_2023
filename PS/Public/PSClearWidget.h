// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PSClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UPSClearWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindCharacterStat(class UPSCharacterStatComponent* NewCharacterStat);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
		TArray<UTexture2D*> img;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void ChangeLevel();

private:
	TWeakObjectPtr<class UPSCharacterStatComponent> CurrentCharacterStat;
	
	UPROPERTY(meta = (BindWidget))
		class UButton* BtnNextStage;

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
};
