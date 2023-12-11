// Fill out your copyright notice in the Description page of Project Settings.


#include "StageSaveActor.h"
#include "PSCharacterStatComponent.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "PSSaveGame.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PSCharacter.h"

// Sets default values
AStageSaveActor::AStageSaveActor()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(80.0f);
	RootComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void AStageSaveActor::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AStageSaveActor::OnOverlapBegin);
}

void AStageSaveActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{		
		UPSSaveGame* LoadGameInstance = Cast<UPSSaveGame>(UGameplayStatics::CreateSaveGameObject(UPSSaveGame::StaticClass()));

		if (LoadGameInstance)
		{
			LoadGameInstance->SaveSlotName = "MySaveGame";
			LoadGameInstance->SaveIndex = 0;

			LoadGameInstance = Cast<UPSSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->SaveIndex));

			if (LoadGameInstance == nullptr) {
				UGameplayStatics::OpenLevel(GetWorld(), TEXT("Stage1")); // 세이브파일이 null일 경우 스테이지1 부터
			}
			else {
				UGameplayStatics::OpenLevel(GetWorld(), FName(LoadGameInstance->SaveStageName)); // 세이브가 있을경우 세이브된 스테이지부터
			}
		}
	}
}