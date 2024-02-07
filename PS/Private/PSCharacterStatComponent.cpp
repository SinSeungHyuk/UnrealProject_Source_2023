// Fill out your copyright notice in the Description page of Project Settings.


#include "PSCharacterStatComponent.h"
#include "PSGameInstance.h"
#include "PSCharacter.h"
#include "PSCharacterAnimInstance.h"
#include "PSPlayerController.h"
#include "PSSaveGame.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UPSCharacterStatComponent::UPSCharacterStatComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
	MaxMP = 100.0f;
	CurrentMP = 0.f;

	isUpgrade = false;
	UpgradeMode = 0; // ó������ ��ų ���׷��̵� x
	// 1: �ִ�ü�� 2:���ݷ� 3:���� 4:�̼� 5:ȸ���� 6:�� 7:ü�� 8:���� 9:���� ���ں��� ��ų ���׷��̵� �Ű�����
}

void UPSCharacterStatComponent::SetNewLevel(int32 newLevel) // �÷��̾� ������ �ٲ𶧸��� ��������
{
	auto PSGameInstance = Cast<UPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CurrentStatData = PSGameInstance->GetPSCharacterData(newLevel);
	if (nullptr != CurrentStatData)
	{
		Level = newLevel;
		NeedEXP = CurrentStatData->NeedEXP; // ���������� �޶����� �ʿ� ����ġ

		OnLevelChanged.Broadcast(); // �������� �Ҷ����� ���� ���� ȣ��
	}
}

void UPSCharacterStatComponent::PSTakeDamage(float Damage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - Damage, 0.0f, CurrentStatData->MaxHP)); // ������ ������ 
}

void UPSCharacterStatComponent::SetHP(float newHP)
{
	CurrentHP = (newHP >= MaxHP) ? MaxHP : newHP; // �ִ�ü�°� ���Ͽ� newHP�� ����ü������ ����
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

void UPSCharacterStatComponent::SetMP(float newMP) {
	CurrentMP = newMP > 100.0f ? 100.0f : newMP; // ���� MP�� newMP�� ����
	OnMPChanged.Broadcast();
}

// -------------------------- Set Property -----------------------------------

void UPSCharacterStatComponent::SetAttack(float newAttack)
{
	CurrentAttack += newAttack;
	if (CurrentAttack >= 75.0f && isUpgrade == false) { // Ư�� ��ġ���� ���� ���׷��̵� �����̶��
		if (UpgradeMode == 0 || UpgradeMode == 2) {
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->PSAnim->UpgradeSkillSet(2);

			isUpgrade = true;
			UpgradeMode = 2;
		}
	}
	OnAttackChanged.Broadcast(); // SetProperty�� ������ ����ɶ����� ��� UI,ĳ���Ϳ� �ݿ��Ǿ����
}
void UPSCharacterStatComponent::SetMaxHP(float newMaxHP)
{
	MaxHP += newMaxHP;
	SetHP(CurrentHP + newMaxHP*0.5f); // MaxHP ���Ҷ����� OnHPChanged ȣ���ؾ��ϹǷ�
	if (MaxHP >= 140.0f && isUpgrade == false) {
		if (UpgradeMode == 0 || UpgradeMode == 1) {
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->PSAnim->UpgradeSkillSet(1);

			isUpgrade = true;
			UpgradeMode = 1;
		}
	}
	OnMaxMPChanged.Broadcast();
}
void UPSCharacterStatComponent::SetArmor(float newArmor)
{
	Armor += newArmor;
	if (Armor >= 0.2f && isUpgrade == false) {
		if (UpgradeMode == 0 || UpgradeMode == 3) {
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->PSAnim->UpgradeSkillSet(3);

			isUpgrade = true;
			UpgradeMode = 3;
		}
	}
	OnArmorChanged.Broadcast();
}
void UPSCharacterStatComponent::SetSpeed(int32 newSpeed)
{
	Speed += newSpeed;
	if (Speed >= 40.0f && isUpgrade == false) {
		if (UpgradeMode == 0 || UpgradeMode == 4) {
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->PSAnim->UpgradeSkillSet(4);

			isUpgrade = true;
			UpgradeMode = 4;
		}
	}
	OnSpeedChanged.Broadcast();
}
void UPSCharacterStatComponent::SetDodge(int32 newDodge)
{
	Dodge += newDodge;
	if (Dodge >= 20.0f && isUpgrade == false) {
		if (UpgradeMode == 0 || UpgradeMode == 5) {
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->PSAnim->UpgradeSkillSet(5);

			isUpgrade = true;
			UpgradeMode = 5;
		}
	}
	OnDodgeChanged.Broadcast();
}
void UPSCharacterStatComponent::SetLuck(int32 newLuck)
{
	Luck += newLuck;
	if (Luck >= 10.0f && isUpgrade == false) {
		if (UpgradeMode == 0 || UpgradeMode == 6) {
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->PSAnim->UpgradeSkillSet(6);

			isUpgrade = true;
			UpgradeMode = 6;
		}
	}
	OnLuckChanged.Broadcast();
}
void UPSCharacterStatComponent::SetHPRegen(float newHPRegen)
{
	HPRegen += newHPRegen;
	if (HPRegen >= 10.0f && isUpgrade == false) {
		if (UpgradeMode == 0 || UpgradeMode == 7) {
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->PSAnim->UpgradeSkillSet(7);

			isUpgrade = true;
			UpgradeMode = 7;
		}
	}
	OnHPRegenChanged.Broadcast();
}
void UPSCharacterStatComponent::SetLifesteal(float newLifesteal)
{
	Lifesteal += newLifesteal;
	if (Lifesteal >= 0.02f && isUpgrade == false) {
		if (UpgradeMode == 0 || UpgradeMode == 8) {
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->PSAnim->UpgradeSkillSet(8);

			isUpgrade = true;
			UpgradeMode = 8;
		}
	}
	OnLifestealChanged.Broadcast();
}
void UPSCharacterStatComponent::SetAttackSpeed(float newAttackSpeed)
{
	AttackSpeed += newAttackSpeed;
	if (AttackSpeed >= 0.3f && isUpgrade == false) {
		if (UpgradeMode == 0 || UpgradeMode == 9) {
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->PSAnim->UpgradeSkillSet(9);

			isUpgrade = true;
			UpgradeMode = 9;
		}
	}
	OnAttackSpeedChanged.Broadcast();
}
void UPSCharacterStatComponent::SetGold(int32 newGold)
{
	Gold += newGold;
	OnGoldChanged.Broadcast();
}
void UPSCharacterStatComponent::SetEXP(float newEXP)
{
	CurrentEXP += newEXP;
	if (CurrentEXP >= NeedEXP)
	{
		CurrentEXP = 0.0f;
		SetNewLevel(Level+1); // ���� ������ SetNewLevel ����
		APSPlayerController* PlayerController = Cast<APSPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		PlayerController->OnLevelUPWidget(); // �������� �Ͽ����Ƿ� ������ UI ����
	}
	OnEXPChanged.Broadcast();
}
void UPSCharacterStatComponent::SetStageName(FString name)
{
	StageName = name;
}

// -------------------------- Get Property -----------------------------------

float UPSCharacterStatComponent::GetHP()
{
	return MaxHP;
}

float UPSCharacterStatComponent::GetCurrentHP()
{
	return CurrentHP;
}

float UPSCharacterStatComponent::GetAttack()
{
	return CurrentAttack;
}
float UPSCharacterStatComponent::GetArmor()
{
	return Armor;
}

int32 UPSCharacterStatComponent::GetSpeed()
{
	return Speed;
}
int32 UPSCharacterStatComponent::GetDodge()
{
	return Dodge;
}

int32 UPSCharacterStatComponent::GetLuck()
{
	return Luck;
}
float UPSCharacterStatComponent::GetHPRegen()
{
	return HPRegen;
}

float UPSCharacterStatComponent::GetLifesteal()
{
	return Lifesteal;
}
float UPSCharacterStatComponent::GetAttackSpeed()
{
	return AttackSpeed;
}

int32 UPSCharacterStatComponent::GetGold()
{
	return Gold;
}
float UPSCharacterStatComponent::GetEXP()
{
	return CurrentEXP;
}

float UPSCharacterStatComponent::GetMP()
{
	return CurrentMP;
}

float UPSCharacterStatComponent::getEXPRatio()
{
	return (CurrentStatData->NeedEXP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentEXP / CurrentStatData->NeedEXP);
}
float UPSCharacterStatComponent::getHPRatio()
{
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}
float UPSCharacterStatComponent::getMPRatio()
{
	return (CurrentMP / MaxMP);
}

float UPSCharacterStatComponent::getIdentityRatio()
{
	return 1.0f+(CurrentMP/100.0f); // Identity�� MP�� ��%���� ���Ǿ����� ���
}

int32 UPSCharacterStatComponent::GetUpgradeMode()
{
	return UpgradeMode;
}

FString UPSCharacterStatComponent::GetStageName()
{
	return StageName;
}

//--------------------------- Save & Load -----------------------------------------

void UPSCharacterStatComponent::Save()
{
	UPSSaveGame* SaveGameInstance = Cast<UPSSaveGame>(UGameplayStatics::CreateSaveGameObject(UPSSaveGame::StaticClass()));
	if (SaveGameInstance)
	{
		/** Save file data **/
		SaveGameInstance->SaveSlotName = "MySaveGame";
		SaveGameInstance->SaveIndex = 0;

		/** Save data **/
		SaveGameInstance->SaveLevel = Level;
		SaveGameInstance->SaveAttack = CurrentAttack;
		SaveGameInstance->SaveMaxHP = MaxHP;
		SaveGameInstance->SaveArmor = Armor;
		SaveGameInstance->SaveSpeed = Speed;
		SaveGameInstance->SaveDodge = Dodge;
		SaveGameInstance->SaveLuck = Luck;
		SaveGameInstance->SaveHPRegen = HPRegen;
		SaveGameInstance->SaveLifesteal = Lifesteal;
		SaveGameInstance->SaveAttackSpeed = AttackSpeed;
		SaveGameInstance->SaveCurrentEXP = CurrentEXP;
		SaveGameInstance->SaveUpgrageMode = UpgradeMode;
		SaveGameInstance->SaveEnemyKills = EnemyKills;
		SaveGameInstance->SaveGold = Gold;  // �����ؾ��ϴ� ��� ������ ����
		SaveGameInstance->SaveStageName = StageName;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGameInstance is nullptr"));
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->SaveIndex); // ���Կ� ����
}

void UPSCharacterStatComponent::Load()
{
	UPSSaveGame* LoadGameInstance = Cast<UPSSaveGame>(UGameplayStatics::CreateSaveGameObject(UPSSaveGame::StaticClass()));

	if (LoadGameInstance)
	{
		LoadGameInstance->SaveSlotName = "MySaveGame";
		LoadGameInstance->SaveIndex = 0;

		LoadGameInstance = Cast<UPSSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->SaveIndex));

		if (LoadGameInstance == nullptr) {
			SetNewLevel(0);
			SetMaxHP(100.0f);
			SetHP(100.0f);
			SetAttack(50.0f);
			SetGold(0);  // ó�� ���۽� ����� ������ ���� ��� �⺻ ����
		}
		else {
			UpgradeMode = LoadGameInstance->SaveUpgrageMode;
			SetNewLevel(LoadGameInstance->SaveLevel);
			SetMaxHP(LoadGameInstance->SaveMaxHP);
			SetHP(LoadGameInstance->SaveMaxHP);
			SetAttack(LoadGameInstance->SaveAttack);
			SetArmor(LoadGameInstance->SaveArmor);
			SetSpeed(LoadGameInstance->SaveSpeed);
			SetDodge(LoadGameInstance->SaveDodge);
			SetLuck(LoadGameInstance->SaveLuck);
			SetHPRegen(LoadGameInstance->SaveHPRegen);
			SetLifesteal(LoadGameInstance->SaveLifesteal);
			SetAttackSpeed(LoadGameInstance->SaveAttackSpeed);
			SetEXP(LoadGameInstance->SaveCurrentEXP);
			SetGold(LoadGameInstance->SaveGold);
			EnemyKills = LoadGameInstance->SaveEnemyKills;
			StageName = LoadGameInstance->SaveStageName;
			// ����Ǿ��ִ� ��� ������ ������ SetProperty
		}
	}
}

void UPSCharacterStatComponent::DeleteSave()
{
	UPSSaveGame* LoadGameInstance = Cast<UPSSaveGame>(UGameplayStatics::CreateSaveGameObject(UPSSaveGame::StaticClass()));

	if (LoadGameInstance)
	{
		LoadGameInstance->SaveSlotName = "MySaveGame";
		LoadGameInstance->SaveIndex = 0;

		LoadGameInstance = Cast<UPSSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->SaveIndex));

		if (LoadGameInstance != nullptr) {
			UGameplayStatics::DeleteGameInSlot("MySaveGame", 0); // �ش� ���ӽ��� ����
		}
	}
}
