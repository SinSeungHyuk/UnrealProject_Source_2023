// Fill out your copyright notice in the Description page of Project Settings.


#include "PSBossAnimInstance.h"

UPSBossAnimInstance::UPSBossAnimInstance()
{

}

void UPSBossAnimInstance::NativeInitializeAnimation()
{
}

void UPSBossAnimInstance::PlaySkillMontage(int32 idx)
{
    if (!Montage_IsPlaying(SkillMontage[idx]))
    {
        Montage_Play(SkillMontage[idx], 1.0f);
    }
}

void UPSBossAnimInstance::AnimNotify_BigSwing()
{
    OnBigSwingDelegate.Broadcast();
}
void UPSBossAnimInstance::AnimNotify_Swing()
{
    OnSwingDelegate.Broadcast();
}
void UPSBossAnimInstance::AnimNotify_SoulSiphon()
{
    OnSoulSiphonDelegate.Broadcast();
}
void UPSBossAnimInstance::AnimNotify_Subjugation()
{
    OnSubjugationDelegate.Broadcast();
}