// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/Button.h"
#include "PSCharacterStatComponent.h"
#include "PSCharacter.h"
#include "Kismet/GameplayStatics.h"

void UMainWidget::NativeConstruct() {
	Super::NativeConstruct();
	
	BtnStart = Cast<UButton>(GetWidgetFromName(TEXT("BtnStart")));
	BtnStart->OnClicked.AddDynamic(this, &UMainWidget::OnBtnStart);
}

void UMainWidget::OnBtnStart()
{
	//APSCharacter* CharacterSt = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	//CharacterSt->CharacterStat->DeleteSave();

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("OpeningMap"));
}