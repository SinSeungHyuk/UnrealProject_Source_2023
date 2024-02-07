// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PSEnemyHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UPSEnemyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UPSEnemyStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UPSEnemyStatComponent> CurrentCharacterStat;

	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
