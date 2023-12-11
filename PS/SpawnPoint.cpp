// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "PSEnemy.h"
#include "PSStageManagementActor.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
ASpawnPoint::ASpawnPoint()
{

}

void ASpawnPoint::TimeOut()
{
	GetWorld()->GetTimerManager().ClearTimer(delay_handle);
	GetWorld()->GetTimerManager().ClearTimer(SpawnHandle);
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(true); // 처음에는 보이지 않음

	StageManagementActor = Cast<APSStageManagementActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APSStageManagementActor::StaticClass()));
	StageManagementActor->OnTimeOutStageDelegate.AddUObject(this, &ASpawnPoint::TimeOut);

	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			SetActorHiddenInGame(false); // Delay만큼의 시간이 지나면 활성화
			// SpawnTime 마다 반복하여 스폰함수 실행
			GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &ASpawnPoint::Spawn, SpawnTime, true);
		}), Delay, false);
}

void ASpawnPoint::EndPlay(EEndPlayReason::Type)
{
	Super::EndPlay(EEndPlayReason::Destroyed);

	GetWorld()->GetTimerManager().ClearTimer(delay_handle);
}

void ASpawnPoint::Spawn() {
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// GetWorld()->SpawnActor<APSEnemy> 로 PSEnemy 타입의 액터 스폰
	GetWorld()->SpawnActor<APSEnemy>(spawn_obj, GetActorLocation(), GetActorRotation(), params);
}