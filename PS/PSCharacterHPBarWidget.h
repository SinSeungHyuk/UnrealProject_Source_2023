// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSCharacterHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UPSCharacterHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindCharacterStat(class UPSCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
	void UpdateMPWidget();
	void UpdateEXPWidget();
	void UpdateCurrentGold();
	void UpdateCurrentLevel();
	void UpdateSkillQColldownBar(float remainingTime);
	void UpdateSkillWColldownBar(float remainingTime);
	void UpdateSkillEColldownBar(float remainingTime);
	void UpdateSkillRColldownBar(float remainingTime);
	void UpdateDashColldownBar(float remainingTime);
	void UpdateTimer();

private:
	TWeakObjectPtr<class UPSCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
		class UProgressBar* HPProgressBar;
	UPROPERTY()
		class UProgressBar* MPProgressBar;
	UPROPERTY()
		class UProgressBar* EXPProgressBar;
	UPROPERTY()
		class UTextBlock* CurrentGold;
	UPROPERTY()
		class UTextBlock* TxtLevel;
	UPROPERTY()
		class UTextBlock* TxtTimer;

	UPROPERTY()
		class UProgressBar* SkillQColldownBar;
	UPROPERTY()
		class UProgressBar* SkillWColldownBar;
	UPROPERTY()
		class UProgressBar* SkillEColldownBar;
	UPROPERTY()
		class UProgressBar* SkillRColldownBar;
	UPROPERTY()
		class UProgressBar* DashColldownBar;

	FTimerHandle TimerHandle;
	int32 Timer;
};
