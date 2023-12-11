// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSStageManagementActor.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnTimeOutStageDelegate);

UCLASS()
class PS_API APSStageManagementActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APSStageManagementActor();

	FOnTimeOutStageDelegate OnTimeOutStageDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "ANextStagePoint")
		TSubclassOf<class ANextStagePoint> spawn_obj;

	void Spawn();

};
