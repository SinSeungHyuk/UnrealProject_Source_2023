// Fill out your copyright notice in the Description page of Project Settings.


#include "NextStagePoint.h"
#include "PSCharacter.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
ANextStagePoint::ANextStagePoint()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(70.0f);
	RootComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void ANextStagePoint::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ANextStagePoint::OnOverlapBegin);
}

void ANextStagePoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->IsA(APSCharacter::StaticClass()))
		{
			APSCharacter* CharacterSt = Cast<APSCharacter>(OtherActor); // 캐릭터 스탯컴포넌트 가져오기
			CharacterSt->PSPC->OnShopWidget();

			Destroy();
		}
	}
}



