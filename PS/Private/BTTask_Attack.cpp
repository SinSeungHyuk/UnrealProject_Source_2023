// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "PSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PSEnemy.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto PSEnemy = Cast<APSEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == PSEnemy)
		return EBTNodeResult::Failed;

	PSEnemy->EnemyAttack();

	return EBTNodeResult::Succeeded;
}
