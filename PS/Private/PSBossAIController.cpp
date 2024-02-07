// Fill out your copyright notice in the Description page of Project Settings.


#include "PSBossAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PSEnemy.h"

const FName APSBossAIController::HomePosKey(TEXT("HomePos"));
const FName APSBossAIController::TargetKey(TEXT("Target"));
const FName APSBossAIController::TargetLocationKey(TEXT("TargetLocation"));
const FName APSBossAIController::PatternKey(TEXT("Pattern"));

APSBossAIController::APSBossAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Enemy/AI/BT_PSEnemyBoss.BT_PSEnemyBoss'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	BehaviorTreeComp = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
}

void APSBossAIController::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([=]() // 람다식 &->레퍼런스 캡쳐, = 로 해야함
		{
			RunBehaviorTree(BTAsset);
			BehaviorTreeComp->StartTree(*BTAsset);
		}), 5.0f, false); // 오프닝 연출을 위해 5초 뒤 AI 실행
}

void APSBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (blackboard)
	{
		//비헤이비어트리에 있는 블랙보드로 초기화
		blackboard->InitializeBlackboard(*BTAsset->BlackboardAsset);
	}

}

void APSBossAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void APSBossAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

UBlackboardComponent* APSBossAIController::GetBlackboard() const
{
	return blackboard;
}