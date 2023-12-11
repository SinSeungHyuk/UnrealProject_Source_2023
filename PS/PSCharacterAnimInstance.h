// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "PSPlayerController.h"
#include "PSCharacterAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnSkillEndCheckDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillQDelegate,int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillWDelegate, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillEDelegate, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillRDelegate, int32);
DECLARE_MULTICAST_DELEGATE(FOnIdentityDelegate);

UCLASS()
class PS_API UPSCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPSCharacterAnimInstance();
	virtual void NativeInitializeAnimation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn;
	UPROPERTY()
		APSPlayerController* PlayerController;

	void PlaySkillMontage(int32 idx,float ratio);

	void UpgradeSkillSet(int32 UpgradeMode);

	FOnSkillEndCheckDelegate OnSkillEndCheck;
	FOnSkillQDelegate OnSkillQ;
	FOnSkillWDelegate OnSkillW;
	FOnSkillEDelegate OnSkillE;
	FOnSkillRDelegate OnSkillR;
	FOnIdentityDelegate OnIdentity;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> SkillMontage;

	UFUNCTION()
	void AnimNotify_SkillEndCheck();
	UFUNCTION()
	void SkillEnd();

	UFUNCTION()
		void AnimNotify_SkillQ();
	UFUNCTION()
		void AnimNotify_SkillW();
	UFUNCTION()
		void AnimNotify_SkillE();
	UFUNCTION()
		void AnimNotify_SkillR();
	UFUNCTION()
		void AnimNotify_Identity();

	void SkillRatioChanged();

	int32 Upgrade;

	float SkillRatio; // 스킬 사용속도
};
