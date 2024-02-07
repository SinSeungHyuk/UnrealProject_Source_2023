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
	UpgradeMode = 0; // 처음에는 스킬 업그레이드 x
	// 1: 최대체력 2:공격력 3:방어력 4:이속 5:회피율 6:운 7:체젠 8:피흡 9:공속 숫자별로 스킬 업그레이드 매개변수
}

void UPSCharacterStatComponent::SetNewLevel(int32 newLevel) // 플레이어 레벨이 바뀔때마다 레벨변경
{
	auto PSGameInstance = Cast<UPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CurrentStatData = PSGameInstance->GetPSCharacterData(newLevel);
	if (nullptr != CurrentStatData)
	{
		Level = newLevel;
		NeedEXP = CurrentStatData->NeedEXP; // 레벨업마다 달라지는 필요 경험치

		OnLevelChanged.Broadcast(); // 레벨업을 할때마다 레벨 변경 호출
	}
}

void UPSCharacterStatComponent::PSTakeDamage(float Damage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - Damage, 0.0f, CurrentStatData->MaxHP)); // 데미지 받을때 
}

void UPSCharacterStatComponent::SetHP(float newHP)
{
	CurrentHP = (newHP >= MaxHP) ? MaxHP : newHP; // 최대체력과 비교하여 newHP를 현재체력으로 결정
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

void UPSCharacterStatComponent::SetMP(float newMP) {
	CurrentMP = newMP > 100.0f ? 100.0f : newMP; // 현재 MP를 newMP로 결정
	OnMPChanged.Broadcast();
}

// -------------------------- Set Property -----------------------------------

void UPSCharacterStatComponent::SetAttack(float newAttack)
{
	CurrentAttack += newAttack;
	if (CurrentAttack >= 75.0f && isUpgrade == false) { // 특정 수치보다 높고 업그레이드 이전이라면
		if (UpgradeMode == 0 || UpgradeMode == 2) {
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->PSAnim->UpgradeSkillSet(2);

			isUpgrade = true;
			UpgradeMode = 2;
		}
	}
	OnAttackChanged.Broadcast(); // SetProperty로 스탯이 변경될때마다 모두 UI,캐릭터에 반영되어야함
}
void UPSCharacterStatComponent::SetMaxHP(float newMaxHP)
{
	MaxHP += newMaxHP;
	SetHP(CurrentHP + newMaxHP*0.5f); // MaxHP 변할때마다 OnHPChanged 호출해야하므로
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
		SetNewLevel(Level+1); // 다음 레벨로 SetNewLevel 실행
		APSPlayerController* PlayerController = Cast<APSPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		PlayerController->OnLevelUPWidget(); // 레벨업을 하였으므로 레벨업 UI 생성
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
	return 1.0f+(CurrentMP/100.0f); // Identity가 MP의 몇%에서 사용되었는지 계산
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
		SaveGameInstance->SaveGold = Gold;  // 저장해야하는 모든 데이터 저장
		SaveGameInstance->SaveStageName = StageName;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGameInstance is nullptr"));
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->SaveIndex); // 슬롯에 저장
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
			SetGold(0);  // 처음 시작시 저장된 정보가 없을 경우 기본 세팅
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
			// 저장되어있는 모든 데이터 가져와 SetProperty
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
			UGameplayStatics::DeleteGameInSlot("MySaveGame", 0); // 해당 게임슬롯 삭제
		}
	}
}
