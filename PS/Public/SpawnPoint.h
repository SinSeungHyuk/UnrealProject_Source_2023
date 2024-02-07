// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class PS_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "SpawnEnemy")
		TSubclassOf<class APSEnemy> spawn_obj;
	UFUNCTION()
		void TimeOut();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type) override;

	void Spawn();
	
private:
	// 몬스터가 스폰하는데 걸리는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "SpawnEnemy")
		float SpawnTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "SpawnEnemy")
		float Delay; // 월드에서 활성화되기까지의 시간

	FTimerHandle SpawnHandle, delay_handle;

	UPROPERTY()
		class APSStageManagementActor* StageManagementActor;

};
