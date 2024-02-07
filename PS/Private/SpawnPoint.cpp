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
	
	SetActorHiddenInGame(true); // ó������ ������ ����

	StageManagementActor = Cast<APSStageManagementActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APSStageManagementActor::StaticClass()));
	StageManagementActor->OnTimeOutStageDelegate.AddUObject(this, &ASpawnPoint::TimeOut);

	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			SetActorHiddenInGame(false); // Delay��ŭ�� �ð��� ������ Ȱ��ȭ
			// SpawnTime ���� �ݺ��Ͽ� �����Լ� ����
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
	// GetWorld()->SpawnActor<APSEnemy> �� PSEnemy Ÿ���� ���� ����
	GetWorld()->SpawnActor<APSEnemy>(spawn_obj, GetActorLocation(), GetActorRotation(), params);
}