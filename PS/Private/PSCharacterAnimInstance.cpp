// Fill out your copyright notice in the Description page of Project Settings.


#include "PSCharacterAnimInstance.h"
#include "PSCharacterStatComponent.h"
#include "PSPlayerController.h"
#include "PSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

UPSCharacterAnimInstance::UPSCharacterAnimInstance()
{
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE1(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillQ_Montage.SkillQ_Montage'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE2(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillW_Montage.SkillW_Montage'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE3(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillE_Montage.SkillE_Montage'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE4(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillR_Montage.SkillR_Montage'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE5(TEXT("AnimMontage'/Game/PSCharacter/Animation/Dash_Montage.Dash_Montage'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE6(TEXT("AnimMontage'/Game/PSCharacter/Animation/Identity_Montage.Identity_Montage'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE7(TEXT("AnimMontage'/Game/PSCharacter/Animation/WE_Montage.WE_Montage'"));
    // 기본 스킬 4개 + 대쉬 + 아이덴티티
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE8(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillQ_Montage_MaxHP.SkillQ_Montage_MaxHP'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE9(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillW_Montage_MaxHP.SkillW_Montage_MaxHP'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE10(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillE_Montage_MaxHP.SkillE_Montage_MaxHP'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE11(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillR_Montage_MaxHP.SkillR_Montage_MaxHP'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE12(TEXT("AnimMontage'/Game/PSCharacter/Animation/Identity_Montage_MaxHP.Identity_Montage_MaxHP'"));

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE13(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillQ_Montage_Attack.SkillQ_Montage_Attack'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE14(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillW_Montage_Attack.SkillW_Montage_Attack'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE15(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillE_Montage_Attack.SkillE_Montage_Attack'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE16(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillR_Montage_Attack.SkillR_Montage_Attack'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE17(TEXT("AnimMontage'/Game/PSCharacter/Animation/Identity_Montage_Attack.Identity_Montage_Attack'"));

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE18(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillQ_Montage_Armor.SkillQ_Montage_Armor'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE19(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillW_Montage_Armor.SkillW_Montage_Armor'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE20(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillE_Montage_Armor.SkillE_Montage_Armor'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE21(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillR_Montage_Armor.SkillR_Montage_Armor'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE22(TEXT("AnimMontage'/Game/PSCharacter/Animation/Identity_Montage_Armor.Identity_Montage_Armor'"));

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE23(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillQ_Montage_Speed.SkillQ_Montage_Speed'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE24(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillW_Montage_Speed.SkillW_Montage_Speed'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE25(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillE_Montage_Speed.SkillE_Montage_Speed'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE26(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillR_Montage_Speed.SkillR_Montage_Speed'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE27(TEXT("AnimMontage'/Game/PSCharacter/Animation/Identity_Montage_Speed.Identity_Montage_Speed'"));

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE28(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillQ_Montage_Dodge.SkillQ_Montage_Dodge'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE29(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillW_Montage_Dodge.SkillW_Montage_Dodge'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE30(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillE_Montage_Dodge.SkillE_Montage_Dodge'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE31(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillR_Montage_Dodge.SkillR_Montage_Dodge'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE32(TEXT("AnimMontage'/Game/PSCharacter/Animation/Identity_Montage_Dodge.Identity_Montage_Dodge'"));

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE33(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillQ_Montage_Luck.SkillQ_Montage_Luck'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE34(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillW_Montage_Luck.SkillW_Montage_Luck'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE35(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillE_Montage_Luck.SkillE_Montage_Luck'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE36(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillR_Montage_Luck.SkillR_Montage_Luck'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE37(TEXT("AnimMontage'/Game/PSCharacter/Animation/Identity_Montage_Luck.Identity_Montage_Luck'"));

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE38(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillQ_Montage_HPRegen.SkillQ_Montage_HPRegen'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE39(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillW_Montage_HPRegen.SkillW_Montage_HPRegen'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE40(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillE_Montage_HPRegen.SkillE_Montage_HPRegen'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE41(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillR_Montage_HPRegen.SkillR_Montage_HPRegen'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE42(TEXT("AnimMontage'/Game/PSCharacter/Animation/Identity_Montage_HPRegen.Identity_Montage_HPRegen'"));

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE43(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillQ_Montage_Lifesteal.SkillQ_Montage_Lifesteal'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE44(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillW_Montage_Lifesteal.SkillW_Montage_Lifesteal'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE45(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillE_Montage_Lifesteal.SkillE_Montage_Lifesteal'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE46(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillR_Montage_Lifesteal.SkillR_Montage_Lifesteal'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE47(TEXT("AnimMontage'/Game/PSCharacter/Animation/Identity_Montage_Lifesteal.Identity_Montage_Lifesteal'"));

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE48(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillQ_Montage_AttackSpeed.SkillQ_Montage_AttackSpeed'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE49(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillW_Montage_AttackSpeed.SkillW_Montage_AttackSpeed'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE50(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillE_Montage_AttackSpeed.SkillE_Montage_AttackSpeed'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE51(TEXT("AnimMontage'/Game/PSCharacter/Animation/SkillR_Montage_AttackSpeed.SkillR_Montage_AttackSpeed'"));
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE52(TEXT("AnimMontage'/Game/PSCharacter/Animation/Identity_Montage_AttackSpeed.Identity_Montage_AttackSpeed'"));
    // 각각의 업그레이드 될 스킬 몽타주들

    if (ATTACK_MONTAGE1.Succeeded())
    {
        SkillMontage.Emplace(ATTACK_MONTAGE1.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE2.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE3.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE4.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE5.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE6.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE7.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE8.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE9.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE10.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE11.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE12.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE13.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE14.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE15.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE16.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE17.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE18.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE19.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE20.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE21.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE22.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE23.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE24.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE25.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE26.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE27.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE28.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE29.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE30.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE31.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE32.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE33.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE34.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE35.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE36.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE37.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE38.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE39.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE40.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE41.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE42.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE43.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE44.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE45.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE46.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE47.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE48.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE49.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE50.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE51.Object);
        SkillMontage.Emplace(ATTACK_MONTAGE52.Object);
    }

    Upgrade = 0;
}

void UPSCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    if (Pawn == nullptr)
    {
        Pawn = TryGetPawnOwner();
        PlayerController = Cast<APSPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    }
    OnSkillEndCheck.AddUObject(this, &UPSCharacterAnimInstance::SkillEnd);
}

void UPSCharacterAnimInstance::PlaySkillMontage(int32 idx,float ratio)
{
    if (!Montage_IsPlaying(SkillMontage[idx]))
    {
        Pawn->GetMovementComponent()->StopMovementImmediately();
        PlayerController->DisableInput(PlayerController);

        Montage_Play(SkillMontage[idx], 1.0f+ratio); // ratio -> 공격속도
    }
}

void UPSCharacterAnimInstance::UpgradeSkillSet(int32 UpgradeMode)
{
    PlayerController->OnSkillUpgradeWidget(UpgradeMode);
    Upgrade = UpgradeMode;

    switch (UpgradeMode) // 업그레이드 상태에 따라 사용하는 스킬 몽타주 변경
    {
    case 1:
        SkillMontage[0] = SkillMontage[7];
        SkillMontage[1] = SkillMontage[8];
        SkillMontage[2] = SkillMontage[9];
        SkillMontage[3] = SkillMontage[10];
        SkillMontage[5] = SkillMontage[11];
        break;
    case 2:
        SkillMontage[0] = SkillMontage[12];
        SkillMontage[1] = SkillMontage[13];
        SkillMontage[2] = SkillMontage[14];
        SkillMontage[3] = SkillMontage[15];
        SkillMontage[5] = SkillMontage[16];
        break;
    case 3:
        SkillMontage[0] = SkillMontage[17];
        SkillMontage[1] = SkillMontage[18];
        SkillMontage[2] = SkillMontage[19];
        SkillMontage[3] = SkillMontage[20];
        SkillMontage[5] = SkillMontage[21];
        break;
    case 4:
        SkillMontage[0] = SkillMontage[22];
        SkillMontage[1] = SkillMontage[23];
        SkillMontage[2] = SkillMontage[24];
        SkillMontage[3] = SkillMontage[25];
        SkillMontage[5] = SkillMontage[26];
        break;
    case 5:
        SkillMontage[0] = SkillMontage[27];
        SkillMontage[1] = SkillMontage[28];
        SkillMontage[2] = SkillMontage[29];
        SkillMontage[3] = SkillMontage[30];
        SkillMontage[5] = SkillMontage[31];
        break;
    case 6:
        SkillMontage[0] = SkillMontage[32];
        SkillMontage[1] = SkillMontage[33];
        SkillMontage[2] = SkillMontage[34];
        SkillMontage[3] = SkillMontage[35];
        SkillMontage[5] = SkillMontage[36];
        break;
    case 7:
        SkillMontage[0] = SkillMontage[37];
        SkillMontage[1] = SkillMontage[38];
        SkillMontage[2] = SkillMontage[39];
        SkillMontage[3] = SkillMontage[40];
        SkillMontage[5] = SkillMontage[41];
        break;
    case 8:
        SkillMontage[0] = SkillMontage[42];
        SkillMontage[1] = SkillMontage[43];
        SkillMontage[2] = SkillMontage[44];
        SkillMontage[3] = SkillMontage[45];
        SkillMontage[5] = SkillMontage[46];
        break;
    case 9:
        SkillMontage[0] = SkillMontage[47];
        SkillMontage[1] = SkillMontage[48];
        SkillMontage[2] = SkillMontage[49];
        SkillMontage[3] = SkillMontage[50];
        SkillMontage[5] = SkillMontage[51];
        break;
    default:
        break;
    }
}

void UPSCharacterAnimInstance::AnimNotify_SkillEndCheck()
{
    OnSkillEndCheck.Broadcast();
}

void UPSCharacterAnimInstance::SkillEnd()
{
    PlayerController->EnableInput(PlayerController);
}

void UPSCharacterAnimInstance::AnimNotify_SkillQ()
{
    OnSkillQ.Broadcast(Upgrade);
}
void UPSCharacterAnimInstance::AnimNotify_SkillW()
{
    OnSkillW.Broadcast(Upgrade);
}
void UPSCharacterAnimInstance::AnimNotify_SkillE()
{
    OnSkillE.Broadcast(Upgrade);
}
void UPSCharacterAnimInstance::AnimNotify_SkillR()
{
    OnSkillR.Broadcast(Upgrade);
}
void UPSCharacterAnimInstance::AnimNotify_Identity()
{
    OnIdentity.Broadcast();
}
