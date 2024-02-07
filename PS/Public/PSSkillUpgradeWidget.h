// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSSkillUpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UPSSkillUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindCharacterStat(int32 UpgradeMode);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
		TArray<UTexture2D*> img;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
		class UTextBlock* TxtUpgradeTitle;
	UPROPERTY()
		class UTextBlock* TxtUpgrade; // Item Price

	UPROPERTY()
		class UImage* ImgStat;

	int32 UpgradeMode;
};
