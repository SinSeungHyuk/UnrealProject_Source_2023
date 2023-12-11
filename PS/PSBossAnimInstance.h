// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PSBossAnimInstance.generated.h"

/**
 */
DECLARE_MULTICAST_DELEGATE(FOnBigSwingDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSwingDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSoulSiphonDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSubjugationDelegate);

UCLASS()
class PS_API UPSBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPSBossAnimInstance();
	virtual void NativeInitializeAnimation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn;

	void PlaySkillMontage(int32 idx);

	FOnBigSwingDelegate OnBigSwingDelegate;
	FOnSwingDelegate OnSwingDelegate;
	FOnSoulSiphonDelegate OnSoulSiphonDelegate;
	FOnSubjugationDelegate OnSubjugationDelegate;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> SkillMontage;

	UFUNCTION()
		void AnimNotify_BigSwing();
	UFUNCTION()
		void AnimNotify_Swing();
	UFUNCTION()
		void AnimNotify_SoulSiphon();
	UFUNCTION()
		void AnimNotify_Subjugation();

};
