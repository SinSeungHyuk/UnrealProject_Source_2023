// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PSEnemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnemyDeadDelegate);

UCLASS()
class PS_API APSEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APSEnemy();
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "BP")
	void Attack(AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "BP")
		void EnemyAttack();
	UFUNCTION(BlueprintImplementableEvent, Category = "BP")
		void Dead();
	UFUNCTION(BlueprintImplementableEvent, Category = "BP")
		void TimeOut();

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UPSEnemyStatComponent* MonsterStat;
	UPROPERTY(EditAnywhere, Category = Stat)
		int32 Level;
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "IsRange")
		bool IsRange;

	FOnEnemyDeadDelegate OnEnemyDeadDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		class APSStageManagementActor* StageManagementActor;

	void DeathCount();

};
