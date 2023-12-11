// Fill out your copyright notice in the Description page of Project Settings.


#include "PSEnemy.h"
#include "PSEnemyStatComponent.h"
#include "PSCharacter.h"
#include "PSCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "PSEnemyHPBarWidget.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PSStageManagementActor.h"

// Sets default values
APSEnemy::APSEnemy()
{

	MonsterStat = CreateDefaultSubobject<UPSEnemyStatComponent>(TEXT("MONSTERSTAT")); // 캐릭터 스탯 붙이기

	AIControllerClass = APSAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/EnemyHPBar.EnemyHPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(100.0f, 50.0f));
	}

	OnEnemyDeadDelegate.AddUObject(this, &APSEnemy::DeathCount);
}

// Called when the game starts or when spawned
void APSEnemy::BeginPlay()
{
	Super::BeginPlay();

	StageManagementActor = Cast<APSStageManagementActor>(UGameplayStatics::GetActorOfClass(GetWorld(),APSStageManagementActor::StaticClass()));
	StageManagementActor->OnTimeOutStageDelegate.AddUObject(this, &APSEnemy::TimeOut);

	MonsterStat->SetNewLevel(Level);
	auto EnemyHPWidget = Cast<UPSEnemyHPBarWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != EnemyHPWidget)
	{
		EnemyHPWidget->BindCharacterStat(MonsterStat);
	}
	GetCharacterMovement()->MaxWalkSpeed=MonsterStat->Speed; // 몬스터 데이터의 스피드 가져오기

	if (Level == 2 || Level == 5 || Level == 8) IsRange = true;
}

void APSEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterStat->OnHPIsZero.AddLambda([this]()->void {
		OnEnemyDeadDelegate.Broadcast();
		Dead();
	});
}

float APSEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	MonsterStat->SetDamage(FinalDamage);
	return FinalDamage;
}

void APSEnemy::Attack(AActor* OtherActor)
{
	UGameplayStatics::ApplyDamage(OtherActor, MonsterStat->Attack, nullptr, this, NULL);
}

void APSEnemy::DeathCount()
{
	APSCharacter* YourCharacter = Cast<APSCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	YourCharacter->CharacterStat->EnemyKills++; // 죽었을 경우 델리게이트를 통해 캐릭터스탯컴포넌트의 킬수가 올라감
}