// Fill out your copyright notice in the Description page of Project Settings.


#include "PSCharacterHPBarWidget.h"
#include "PSCharacterStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PSPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UPSCharacterHPBarWidget::BindCharacterStat(UPSCharacterStatComponent* NewCharacterStat)
{
	if (nullptr != NewCharacterStat)
	{
		CurrentCharacterStat = NewCharacterStat;
		NewCharacterStat->OnHPChanged.AddUObject(this, &UPSCharacterHPBarWidget::UpdateHPWidget);
		NewCharacterStat->OnMPChanged.AddUObject(this, &UPSCharacterHPBarWidget::UpdateMPWidget);
		NewCharacterStat->OnEXPChanged.AddUObject(this, &UPSCharacterHPBarWidget::UpdateEXPWidget);
		NewCharacterStat->OnGoldChanged.AddUObject(this, &UPSCharacterHPBarWidget::UpdateCurrentGold);
		NewCharacterStat->OnLevelChanged.AddUObject(this, &UPSCharacterHPBarWidget::UpdateCurrentLevel);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("character state widget error"));
}
 
void UPSCharacterHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APSPlayerController* PSPC = Cast<APSPlayerController>(UGameplayStatics::GetPlayerController(this, 0)); // �÷��̾� ��Ʈ�ѷ� ��������

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("CharacterHPBar")));
	UpdateHPWidget();

	MPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("CharacterMPBar")));
	UpdateMPWidget();

	EXPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("CharacterEXPBar")));
	UpdateEXPWidget();

	CurrentGold = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtGold")));
	UpdateCurrentGold();

	TxtLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtLevel")));
	UpdateCurrentLevel();

	TxtTimer = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextTimer")));
	Timer = 300;   // ���� �ð� ����
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPSCharacterHPBarWidget::UpdateTimer, 1.0f, true); // 1�ʸ��� Ÿ�̸� ����

	// ó�� ���۽� HPBar,MPBar,EXPBar,Gold,Level,Timer ������Ʈ �ѹ� ���ְ� ����

	SkillQColldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("SkillQColldownBar")));
	PSPC->OnSkillQCooldown.AddUObject(this, &UPSCharacterHPBarWidget::UpdateSkillQColldownBar);

	SkillWColldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("SkillWColldownBar")));
	PSPC->OnSkillWCooldown.AddUObject(this, &UPSCharacterHPBarWidget::UpdateSkillWColldownBar);

	SkillEColldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("SkillEColldownBar")));
	PSPC->OnSkillECooldown.AddUObject(this, &UPSCharacterHPBarWidget::UpdateSkillEColldownBar);

	SkillRColldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("SkillRColldownBar")));
	PSPC->OnSkillRCooldown.AddUObject(this, &UPSCharacterHPBarWidget::UpdateSkillRColldownBar);

	DashColldownBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("DashColldownBar")));
	PSPC->OnDashCooldown.AddUObject(this, &UPSCharacterHPBarWidget::UpdateDashColldownBar);
	// ��ų ��ٿ� ������Ʈ�� ���� ��������Ʈ ���
}

void UPSCharacterHPBarWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->getHPRatio()); // HP ������ŭ ���α׷����� ������Ʈ
		}
	}
}

void UPSCharacterHPBarWidget::UpdateMPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != MPProgressBar)
		{
			MPProgressBar->SetPercent(CurrentCharacterStat->getMPRatio()); // MP ������ŭ ���α׷����� ������Ʈ
		}
	}
}

void UPSCharacterHPBarWidget::UpdateEXPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != EXPProgressBar)
		{
			EXPProgressBar->SetPercent(CurrentCharacterStat->getEXPRatio()); // EXP ������ŭ ���α׷����� ������Ʈ
		}
	}
}

void UPSCharacterHPBarWidget::UpdateTimer()
{
	Timer--;
	if (Timer <= 0) Timer = 0; // �ð��� 0�� �������� 0���� ����
	TxtTimer->SetText(FText::FromString(FString::FromInt(Timer)));
}

void UPSCharacterHPBarWidget::UpdateCurrentGold()
{
	if (CurrentCharacterStat.IsValid())
	{
		CurrentGold->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
	}
}

void UPSCharacterHPBarWidget::UpdateCurrentLevel()
{
	if (CurrentCharacterStat.IsValid())
	{
		TxtLevel->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->Level+1)));
	}
}

void UPSCharacterHPBarWidget::UpdateSkillQColldownBar(float remainingTime) // �Ű����� ���� �ð� ���� 
{
	if (nullptr != SkillQColldownBar)
	{
		SkillQColldownBar->SetPercent(remainingTime); // ���� �ð��� ������ŭ SetPercent�� ��ٿ� ǥ��
	}
}

void UPSCharacterHPBarWidget::UpdateSkillWColldownBar(float remainingTime)
{
	if (nullptr != SkillWColldownBar)
	{
		SkillWColldownBar->SetPercent(remainingTime);
	}
}

void UPSCharacterHPBarWidget::UpdateSkillEColldownBar(float remainingTime)
{
	if (nullptr != SkillEColldownBar)
	{
		SkillEColldownBar->SetPercent(remainingTime);
	}
}

void UPSCharacterHPBarWidget::UpdateSkillRColldownBar(float remainingTime)
{
	if (nullptr != SkillRColldownBar)
	{
		SkillRColldownBar->SetPercent(remainingTime);
	}
}

void UPSCharacterHPBarWidget::UpdateDashColldownBar(float remainingTime)
{
	if (nullptr != DashColldownBar)
	{
		DashColldownBar->SetPercent(remainingTime);
	}
}