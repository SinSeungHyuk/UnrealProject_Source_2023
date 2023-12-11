// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceMine.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "PSCharacter.h"
#include "Particles/ParticleSystem.h"
#include "PSCharacterStatComponent.h"
#include "PSStageManagementActor.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
ASpaceMine::ASpaceMine()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(60.0f);
	RootComponent = CollisionComponent;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Space(TEXT("ParticleSystem'/Game/FantasyMagic/Particles/PS_StarsMine.PS_StarsMine'"));
	if (Space.Succeeded())
	{
		SpaceParticle = Space.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Ray(TEXT("ParticleSystem'/Game/FantasyMagic/Particles/PS_RayCast.PS_RayCast'"));
	if (Ray.Succeeded())
	{
		RayCastParticle = Ray.Object;
	}
}

// Called when the game starts or when spawned
void ASpaceMine::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpaceMine::OnOverlapBegin);

	StageManagementActor = Cast<APSStageManagementActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APSStageManagementActor::StaticClass()));
	StageManagementActor->OnTimeOutStageDelegate.AddUObject(this, &ASpaceMine::TimeOut);

	GameStatic->SpawnEmitterAtLocation(GetWorld(), RayCastParticle, GetActorLocation(), GetActorRotation(), FVector(2.0f, 2.0f, 2.0f));
}

void ASpaceMine::TimeOut()
{
	Destroy();
}

void ASpaceMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->IsA(APSCharacter::StaticClass()))
		{
			GameStatic->SpawnEmitterAtLocation(GetWorld(), SpaceParticle, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z+20.0f), GetActorRotation(),FVector(3.f,3.0f,3.0f));

			FTimerHandle myTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					OnMine(); // 4초 뒤 함정폭발 함수 실행
				}), 4.0f, false);

			SetActorHiddenInGame(true);
			CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ASpaceMine::OnMine()
{
	UGameplayStatics::PlaySound2D(this, OnMineSound, 1.f);

	FVector StartLoc = GetActorLocation();
	FVector TargetLoc = GetActorLocation();

	TArray<AActor*> IngoreActors;
	IngoreActors.Add(this);

	TArray<FHitResult> HitRetArray;

	bool isHit = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), StartLoc, TargetLoc, 400.f, TEXT("BlockAll"), false, IngoreActors, EDrawDebugTrace::Type::None, HitRetArray, true);
	if (isHit)
	{
		for (FHitResult Hit : HitRetArray)
		{
			if (AActor* Actor = Hit.Actor.Get())
			{
				if (Actor->IsA(APSCharacter::StaticClass())) {
					UGameplayStatics::ApplyDamage(Actor, 50.0f, nullptr, this, NULL);
					Destroy();
				}
			}
		}
	}
}


