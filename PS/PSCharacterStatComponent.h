#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PSCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEXPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnGoldChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLevelChangedDelegate);

DECLARE_MULTICAST_DELEGATE(FOnAttackChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMaxHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnArmorChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSpeedChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDodgeChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLuckChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPRegenChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackSpeedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLifestealChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PS_API UPSCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPSCharacterStatComponent();

public:

	void SetNewLevel(int32 Level);
	void PSTakeDamage(float Damage); // 받은 데미지 처리
	void SetHP(float newHP);
	void SetAttack(float newAttack);
	void SetMaxHP(float newMaxHP);
	void SetArmor(float newArmor);
	void SetSpeed(int32 newSpeed);
	void SetDodge(int32 newDodge);
	void SetLuck(int32 newLuck);
	void SetHPRegen(float newHPRegen);
	void SetLifesteal(float newLifesteal);
	void SetAttackSpeed(float newAttackSpeed);
	void SetGold(int32 newGold);
	void SetMP(float newMP);
	void SetEXP(float newEXP);
	void SetStageName(FString StageName);

	float GetHP();
	float GetCurrentHP();
	float GetAttack();
	float GetArmor();
	int32 GetSpeed();
	int32 GetDodge();
	int32 GetLuck();
	float GetHPRegen();
	float GetLifesteal();
	float GetAttackSpeed();
	int32 GetGold();
	float GetEXP();
	float GetMP();
	float getHPRatio(); // 체력바 UI에 활용
	float getMPRatio(); // MP바 UI에 활용
	float getEXPRatio(); // 경험치바 UI에 활용
	float getIdentityRatio(); // 아이덴티티 게이지
	int32 GetUpgradeMode(); // 몇번째 스킬이 업그레이드 됐는지 UI에 전달
	FString GetStageName();
	
	void Save();
	void Load();
	void DeleteSave();

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;
	FOnMPChangedDelegate OnMPChanged;
	FOnEXPChangedDelegate OnEXPChanged;
	FOnGoldChangedDelegate OnGoldChanged;
	FOnLevelChangedDelegate OnLevelChanged;

	FOnAttackChangedDelegate OnAttackChanged;
	FOnMaxHPChangedDelegate OnMaxMPChanged;
	FOnArmorChangedDelegate OnArmorChanged;
	FOnSpeedChangedDelegate OnSpeedChanged;
	FOnDodgeChangedDelegate OnDodgeChanged;
	FOnLuckChangedDelegate OnLuckChanged;
	FOnHPRegenChangedDelegate OnHPRegenChanged;
	FOnLifestealChangedDelegate OnLifestealChanged;
	FOnAttackSpeedDelegate OnAttackSpeedChanged;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float NeedEXP;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 CurrentAttack;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float MaxHP;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHP;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float MaxMP;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentMP;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float Armor;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Speed;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Dodge;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Luck;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float HPRegen;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float Lifesteal;
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float AttackSpeed;

	FString StageName;
	int32 EnemyKills;
	float CurrentEXP;
	int32 Gold;
private:
	float IdentityRatio;

	struct FPSCharacterData* CurrentStatData = nullptr;

	int32 UpgradeMode;
	bool isUpgrade;
};