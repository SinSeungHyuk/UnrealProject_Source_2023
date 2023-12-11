// Fill out your copyright notice in the Description page of Project Settings.


#include "PSStageManagementActor.h"
#include "NextStagePoint.h"
#include "SpawnPoint.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
APSStageManagementActor::APSStageManagementActor()
{

}

void APSStageManagementActor::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([=]()
		{
			OnTimeOutStageDelegate.Broadcast();
			Spawn(); // 다음 스테이지로 가는 포탈 생성
		}), 300.0f, false); // 300초가 지나면 스테이지 클리어, TimeOut() 발동
}

void APSStageManagementActor::Spawn()
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<ANextStagePoint>(spawn_obj, GetActorLocation(), GetActorRotation(), params);
}
