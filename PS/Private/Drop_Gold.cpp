// Fill out your copyright notice in the Description page of Project Settings.


#include "Drop_Gold.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "PSCharacter.h"
#include "PSCharacterStatComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
ADrop_Gold::ADrop_Gold()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(40.0f);
	RootComponent = CollisionComponent;

}

// Called when the game starts or when spawned
void ADrop_Gold::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADrop_Gold::OnOverlapBegin);
}

void ADrop_Gold::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->IsA(APSCharacter::StaticClass()))
		{
			APSCharacter* CharacterSt = Cast<APSCharacter>(OtherActor); // 캐릭터 스탯컴포넌트 가져오기
			CharacterSt->CharacterStat->SetEXP(1); // 경험치 1 증가
			BonusGold = CharacterSt->CharacterStat->GetLuck(); 
			CharacterSt->CharacterStat->SetGold(10+ BonusGold); // 캐릭터 Luck 스탯만큼 골드 증가
			UGameplayStatics::PlaySound2D(this, Sound, 1.f);

			Destroy();
		}
	}
}
