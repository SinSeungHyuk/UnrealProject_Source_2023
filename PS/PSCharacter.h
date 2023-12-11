// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "PSPlayerController.h"
#include "PSCharacter.generated.h"

class UDataTable;

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSkillData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
		float SkillQCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
		float SkillWCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
		float SkillECooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
		float SkillRCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
		float DashCooldown;
};

class UParticleSystem;

UCLASS(Blueprintable)
class APSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APSCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UPSCharacterStatComponent* CharacterStat;
	UPROPERTY()
		class UPSCharacterAnimInstance* PSAnim;
	UPROPERTY()
		APSPlayerController* PSPC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Identity")
		TSubclassOf<class AIdentityProjectile> spawn_obj; // 아이덴티티 사용시 생성되는 프로젝타일
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Anim")
		UAnimSequence* DeathAnim;

	FSkillData* GetSkillData(int32 SkillNum);
	void TurnToMouseCursor();

	void SkillQ(int32 UpgradeMode);
	void SkillW(int32 UpgradeMode);
	void SkillE(int32 UpgradeMode);
	void SkillR(int32 UpgradeMode);
	void SkillRFollow(int32 UpgradeMode);
	void Identity();
	float getIdentityRatio();

	FTimerHandle UnusedHandle, HPRegenHandle;
	void Dash(float DashDistance);
	void StopDashing();
	void ResetDash();
	bool OnDash;  // 대쉬 구현하기 위한 함수 3개와 프로퍼티

	void SetCharacterSpeed();
	void SetCharacterHPRegen();
	void SetCharacterLifesteal(float dmg);

	UFUNCTION(BlueprintCallable,Category="Blueprint")
	void DropHealing();
	UFUNCTION(BlueprintCallable, Category = "Blueprint")
		void DropSpeedUp();
	UFUNCTION(BlueprintCallable, Category = "Blueprint")
		void DropAttackUp();
	UFUNCTION(BlueprintCallable, Category = "Blueprint")
		void DropIdentity();

	UFUNCTION(BlueprintCallable, Category = "Blueprint")
		void SaveStageName(FString StageName);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY()
		UDataTable* SkillDataTable;
	struct FSkillData* CurrentSkillData;

	UPROPERTY(EditAnywhere, Category = "HitEffect")
		UParticleSystem* HitEffect;

	UPROPERTY()
		class UParticleSystem* FireParticle;
	UPROPERTY()
		class UParticleSystem* WindParticle;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* OurParticleSystem;

	void CalcDamage(); // 데미지 계산 함수

	float SkillQDmg;
	float SkillWDmg;
	float SkillEDmg;
	float SkillRDmg;
	float IdentityDmg;// 캐릭터 스킬의 데미지를 미리 계산

	void DeadPlayer(); // 플레이어 캐릭터의 사망
	bool isDead;
	UPROPERTY()
		class ASpawnPoint* SpawnPoint;
};

