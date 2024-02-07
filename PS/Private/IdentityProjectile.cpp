// Fill out your copyright notice in the Description page of Project Settings.


#include "IdentityProjectile.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PSCharacter.h"
#include "PSCharacterStatComponent.h"
#include "PSEnemy.h"

// Sets default values
AIdentityProjectile::AIdentityProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(100.0f);
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 1600.0f;
	ProjectileMovementComponent->MaxSpeed = 1600.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

void AIdentityProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag("Enemy"))
		{
			APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			Character->SetCharacterLifesteal(Dmg * Character->CharacterStat->GetLifesteal());
			UGameplayStatics::ApplyDamage(OtherActor, Dmg, nullptr, this, NULL);
		}
	}
}

// Called when the game starts or when spawned
void AIdentityProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AIdentityProjectile::OnOverlapBegin);

	APSCharacter* Character = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Dmg = Character->getIdentityRatio(); // 캐릭터에서 미리 계산된 데미지 가져오기
}