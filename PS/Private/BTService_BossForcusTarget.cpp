// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BossForcusTarget.h"
#include "PSCharacter.h"
#include "PSBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PSEnemyBoss.h"
#include "Kismet/GameplayStatics.h"

UBTService_BossForcusTarget::UBTService_BossForcusTarget()
{

}

void UBTService_BossForcusTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto Controller = Cast<APSBossAIController>(OwnerComp.GetAIOwner());
	APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	// TargetLocationKey -> 플레이어의 위치
	Controller->GetBlackboard()->SetValueAsVector(APSBossAIController::TargetLocationKey, Character->GetActorLocation());
	Controller->GetBlackboard()->SetValueAsObject(APSBossAIController::TargetKey, Character);

	auto PSEnemy = Cast<APSEnemyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	auto Target = Cast<APSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APSBossAIController::TargetKey));

	// 플레이어의 방향으로 바라보기 위해 벡터 계산
	FVector LookVector = Target->GetActorLocation() - PSEnemy->GetActorLocation();

	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	PSEnemy->SetActorRotation(TargetRot); // 플레이어 방향으로 Pawn 회전
}