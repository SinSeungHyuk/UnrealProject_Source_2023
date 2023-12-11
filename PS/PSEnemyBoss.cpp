// Fill out your copyright notice in the Description page of Project Settings.


#include "PSEnemyBoss.h"
#include "PSEnemyStatComponent.h"
#include "PSCharacter.h"
#include "PSCharacterStatComponent.h"
#include "PSPlayerController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SpawnPoint.h"
#include "PSBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "PSEnemyHPBarWidget.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PSBossAnimInstance.h"
#include "PSStageManagementActor.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APSEnemyBoss::APSEnemyBoss()
{
	MonsterStat = CreateDefaultSubobject<UPSEnemyStatComponent>(TEXT("MONSTERSTAT")); // 캐릭터 스탯 붙이기

	AIControllerClass = APSBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/EnemyHPBar.EnemyHPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(200.0f, 75.0f));
	}
}

// Called when the game starts or when spawned
void APSEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	StageManagementActor = Cast<APSStageManagementActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APSStageManagementActor::StaticClass()));
	StageManagementActor->OnTimeOutStageDelegate.AddUObject(this, &APSEnemyBoss::TimeOut);

	MonsterStat->SetNewLevel(Level);
	auto EnemyHPWidget = Cast<UPSEnemyHPBarWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != EnemyHPWidget)
	{
		EnemyHPWidget->BindCharacterStat(MonsterStat);
	}
	GetCharacterMovement()->MaxWalkSpeed = MonsterStat->Speed; // 몬스터 데이터의 스피드 가져오기
}

void APSEnemyBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PSAnim = Cast<UPSBossAnimInstance>(GetMesh()->GetAnimInstance());
	PSAnim->OnBigSwingDelegate.AddUObject(this, &APSEnemyBoss::BigSwing);
	PSAnim->OnSwingDelegate.AddUObject(this, &APSEnemyBoss::Swing);
	PSAnim->OnSoulSiphonDelegate.AddUObject(this, &APSEnemyBoss::SoulSiphon);
	PSAnim->OnSubjugationDelegate.AddUObject(this, &APSEnemyBoss::Subjugation);

	MonsterStat->OnHPIsZero.AddLambda([this]()->void {
		APSCharacter* CharacterSt = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		CharacterSt->CharacterStat->Save(); // 엔딩장면으로 넘어가기 전 세이브
		SpawnPoint = Cast<ASpawnPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnPoint::StaticClass()));
		SpawnPoint->TimeOut();
		Dead();
		});
}

float APSEnemyBoss::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	MonsterStat->SetDamage(FinalDamage);
	return FinalDamage;
}

void APSEnemyBoss::EnemyAttack(int32 Pattern)
{
	PSAnim->PlaySkillMontage(Pattern);

	switch (Pattern)
	{
	case 0: // 돌진패턴
		Dash();
		break;
	case 2: // 오브젝트 생성패턴
		UltimateSwing();
		break;
	default:
		break;
	}
}

void APSEnemyBoss::BigSwing()
{
	FVector StartLoc = GetMesh()->GetSocketLocation(TEXT("SwingTraceSocket"));
	FVector TargetLoc = GetMesh()->GetSocketLocation(TEXT("SwingTraceSocket"));

	TArray<AActor*> IngoreActors;
	IngoreActors.Add(this);

	TArray<FHitResult> HitRetArray;

	bool isHit = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), StartLoc, TargetLoc, 200.0f, TEXT("BlockAll"), false, IngoreActors, EDrawDebugTrace::Type::None, HitRetArray, true);
	if (isHit)
	{
		for (FHitResult Hit : HitRetArray)
		{
			if (AActor* Actor = Hit.Actor.Get())
			{
				if (Actor->ActorHasTag("Player")) {
					UGameplayStatics::ApplyDamage(Actor, 35.0f, nullptr, this, NULL);
				}
			}
		}
	}
}
void APSEnemyBoss::Swing()
{
	{
		FVector StartLoc = GetMesh()->GetSocketLocation(TEXT("SwingTraceSocket"));
		FVector TargetLoc = GetMesh()->GetSocketLocation(TEXT("SwingTraceSocket"));

		TArray<AActor*> IngoreActors;
		IngoreActors.Add(this);

		TArray<FHitResult> HitRetArray;

		bool isHit = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), StartLoc, TargetLoc, 150.0f, TEXT("BlockAll"), false, IngoreActors, EDrawDebugTrace::Type::None, HitRetArray, true);
		if (isHit)
		{
			for (FHitResult Hit : HitRetArray)
			{
				if (AActor* Actor = Hit.Actor.Get())
				{
					if (Actor->ActorHasTag("Player")) {
						UGameplayStatics::ApplyDamage(Actor, 20.0f, nullptr, this, NULL);
					}
				}
			}
		}
	}
}

void APSEnemyBoss::Dash()
{
	if (!OnDash) {
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		LaunchCharacter(GetActorForwardVector() * 1500.0f, true, false);
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // ECC_GameTraceChannel1 = Enemy만 통과하는 채널
		OnDash = true;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &APSEnemyBoss::StopDashing, 1.0f, false); // 0.1초동안 캐릭터 Launch
	}
}
void APSEnemyBoss::StopDashing() {
	GetCharacterMovement()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2); // ECC_GameTraceChannel2 = Player (콜리전 활성화)
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APSEnemyBoss::ResetDash, 1.1f, false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}
void APSEnemyBoss::ResetDash() {
	OnDash = false;
}

void APSEnemyBoss::TimeOut()
{
	auto PlayerController = Cast<APSPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	PlayerController->OnFailWidget();
}