// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossAttack.h"
#include "PSBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PSEnemyBoss.h"

UBTTask_BossAttack::UBTTask_BossAttack()
{
	NodeName = TEXT("BossAttack");

}

EBTNodeResult::Type UBTTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Controller = Cast<APSBossAIController>(OwnerComp.GetAIOwner());

	// PatternKey -> 보스의 패턴
	int32 Pattern = Controller->GetBlackboard()->GetValueAsInt(APSBossAIController::PatternKey);

	auto PSEnemy = Cast<APSEnemyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == PSEnemy)
		return EBTNodeResult::Failed;

	PSEnemy->EnemyAttack(Pattern); // 패턴을 매개변수로 넘겨줌

	return EBTNodeResult::Succeeded;
}

