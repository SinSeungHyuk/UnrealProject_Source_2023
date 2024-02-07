// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeEnemyProjectile.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PSCharacter.h"
#include "PSEnemy.h"

// Sets default values
ARangeEnemyProjectile::ARangeEnemyProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(20.0f);
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 450.0f;
	ProjectileMovementComponent->MaxSpeed = 450.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	InitialLifeSpan = 10.0f;

}

void ARangeEnemyProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->IsA(APSCharacter::StaticClass()))
		{
			UGameplayStatics::ApplyDamage(OtherActor, Dmg, nullptr, this, NULL); // 데미지는 블루프린트에서 정의
			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void ARangeEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ARangeEnemyProjectile::OnOverlapBegin);
	
}
