// Fill out your copyright notice in the Description page of Project Settings.


#include "DropBuffSpawnPoint.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "PSCharacter.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
ADropBuffSpawnPoint::ADropBuffSpawnPoint()
{
	isCreating = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(60.0f);
	RootComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void ADropBuffSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADropBuffSpawnPoint::OnOverlapBegin);

	Spawn();
}

void ADropBuffSpawnPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->IsA(APSCharacter::StaticClass()))
		{
			if (isCreating == false) {
				Spawn();
				isCreating = true; // 스폰포인트와 부딪히는 순간 생성 시작, 생성중에는 생성x
			}
		}
	}
}

void ADropBuffSpawnPoint::Spawn()
{
	FTimerHandle delay_handle;
	GetWorld()->GetTimerManager().SetTimer(delay_handle, FTimerDelegate::CreateLambda([&]()
		{
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			// GetWorld()->SpawnActor<AActor> 로 AActor 타입의 액터 스폰
			GetWorld()->SpawnActor<AActor>(spawn_obj, GetActorLocation(), GetActorRotation(), params);

			isCreating = false;
		}), 30.0f, false); // 30초마다 생성
}

