// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ForcusTarget.h"
#include "PSCharacter.h"
#include "PSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PSEnemy.h"
#include "Kismet/GameplayStatics.h"


UBTService_ForcusTarget::UBTService_ForcusTarget()
{

}

void UBTService_ForcusTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto Controller = Cast<APSAIController>(OwnerComp.GetAIOwner());
	APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	// TargetLocationKey -> �÷��̾��� ��ġ
	Controller->GetBlackboard()->SetValueAsVector(APSAIController::TargetLocationKey, Character->GetActorLocation());
	Controller->GetBlackboard()->SetValueAsObject(APSAIController::TargetKey, Character);

	auto PSEnemy = Cast<APSEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	auto Target = Cast<APSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APSAIController::TargetKey));

	// �÷��̾��� �������� �ٶ󺸱� ���� ���� ���
	FVector LookVector = Target->GetActorLocation() - PSEnemy->GetActorLocation();

	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); 
	PSEnemy->SetActorRotation(TargetRot); // �÷��̾� �������� Pawn ȸ��
}
