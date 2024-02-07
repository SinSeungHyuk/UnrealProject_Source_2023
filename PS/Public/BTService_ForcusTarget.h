// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ForcusTarget.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UBTService_ForcusTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_ForcusTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
