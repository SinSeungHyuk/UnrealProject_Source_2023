// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PSEnemyBoss.generated.h"

UCLASS()
class PS_API APSEnemyBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APSEnemyBoss();
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void EnemyAttack(int32 Pattern);
	UFUNCTION(BlueprintImplementableEvent, Category = "BP")
		void Dead();
	UFUNCTION(BlueprintImplementableEvent, Category = "BP")
		void SoulSiphon();
	UFUNCTION(BlueprintImplementableEvent, Category = "BP")
		void Subjugation();
	UFUNCTION(BlueprintImplementableEvent, Category = "BP")
		void UltimateSwing();

	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UPSEnemyStatComponent* MonsterStat;
	UPROPERTY(EditAnywhere, Category = Stat)
		int32 Level;
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;
	UPROPERTY()
		class UPSBossAnimInstance* PSAnim;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		class APSStageManagementActor* StageManagementActor;
	UPROPERTY()
		class APSEnemy* enemy;
	UPROPERTY()
		class ASpawnPoint* SpawnPoint;

	void BigSwing();
	void Swing();

	FTimerHandle UnusedHandle;
	void Dash();
	void StopDashing();
	void ResetDash();
	bool OnDash;  // 대쉬 구현하기 위한 함수 3개와 프로퍼티

	void TimeOut();

};
