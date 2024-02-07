// Fill out your copyright notice in the Description page of Project Settings.


#include "PSAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PSEnemy.h"

const FName APSAIController::HomePosKey(TEXT("HomePos"));
const FName APSAIController::TargetKey(TEXT("Target"));
const FName APSAIController::TargetLocationKey(TEXT("TargetLocation"));

APSAIController::APSAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Enemy/AI/BT_PSEnemy.BT_PSEnemy'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	BehaviorTreeComp = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
}

void APSAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BTAsset);
	BehaviorTreeComp->StartTree(*BTAsset);
}

void APSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (blackboard)
	{
		//비헤이비어트리에 있는 블랙보드로 초기화
		blackboard->InitializeBlackboard(*BTAsset->BlackboardAsset);
	}

}

void APSAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void APSAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

UBlackboardComponent* APSAIController::GetBlackboard() const
{
	return blackboard;
}