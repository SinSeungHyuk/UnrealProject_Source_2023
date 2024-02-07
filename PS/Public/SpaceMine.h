// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundWave.h"
#include "GameFramework/Actor.h"
#include "SpaceMine.generated.h"

UCLASS()
class PS_API ASpaceMine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpaceMine();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		class USoundWave* OnMineSound;
	UFUNCTION()
		void TimeOut();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
		class USphereComponent* CollisionComponent;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnMine();

	UPROPERTY()
		class UParticleSystem* SpaceParticle;
	UPROPERTY()
		class UParticleSystem* RayCastParticle;

	UPROPERTY()
		class UGameplayStatics* GameStatic;

	UPROPERTY()
		class APSStageManagementActor* StageManagementActor;

};
