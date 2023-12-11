// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PSAIController.generated.h"

/**
 * 
 */
UCLASS()
class PS_API APSAIController : public AAIController
{
	GENERATED_BODY()
public:
	APSAIController(FObjectInitializer const& object_initializer);
	void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	class UBlackboardComponent* GetBlackboard() const;

	static const FName HomePosKey;
	static const FName TargetKey;
	static const FName TargetLocationKey;

	void StopAI();

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBehaviorTreeComponent* BehaviorTreeComp;
	class UBlackboardComponent* blackboard;
};
