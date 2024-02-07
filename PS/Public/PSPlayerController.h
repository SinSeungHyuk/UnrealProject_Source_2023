// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PSLevelUPWidget.h"
#include "PSCharacterHPBarWidget.h"
#include "PSClearWidget.h"
#include "PSShopWidget.h"
#include "PSSkillUpgradeWidget.h"
#include "PSPauseWidget.h"
#include "PSPlayerController.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillQCooldown, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillWCooldown, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillECooldown, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillRCooldown, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDashCooldown, float);

UCLASS()
class APSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APSPlayerController();

	void OnLevelUPWidget();
	void OnShopWidget();
	void OnSkillUpgradeWidget(int32 UpgradeMode);
	void OnPauseWidget();
	void OnClearWidget();
	void OnFailWidget();

	TSubclassOf<UPSCharacterHPBarWidget> HPBarWidgetClass;
	UPSCharacterHPBarWidget* HPBarWidget;

	TSubclassOf<UPSLevelUPWidget> UpgradeWidgetClass;
	UPSLevelUPWidget* UpgradeWidget;

	TSubclassOf<UPSShopWidget> ShopWidgetClass;
	UPSShopWidget* ShopWidget;

	TSubclassOf<UPSSkillUpgradeWidget> SkillUpgradeWidgetClass;
	UPSSkillUpgradeWidget* SkillUpgradeWidget;

	TSubclassOf<UPSPauseWidget> PauseWidgetClass;
	UPSPauseWidget* PauseWidget;

	TSubclassOf<UPSClearWidget> ClearWidgetClass;
	UPSClearWidget* ClearWidget;

	TSubclassOf<UPSClearWidget> FailWidgetClass;
	UPSClearWidget* FailWidget;

	FOnSkillQCooldown OnSkillQCooldown;
	FOnSkillWCooldown OnSkillWCooldown;
	FOnSkillECooldown OnSkillECooldown;
	FOnSkillRCooldown OnSkillRCooldown;
	FOnDashCooldown OnDashCooldown;

	float IdentityRatio;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	bool bStartMoveToMouseCursor;

	FTimerHandle SkillQTimer, SkillWTimer, SkillETimer, SkillRTimer,DashTimer,DashCooldownTimer, SkillQCooldownTimer, SkillWCooldownTimer, SkillECooldownTimer, SkillRCooldownTimer;

	void SkillQ();
	void SkillW();
	void SkillE();
	void SkillR();
	void Dash();
	void Identity();

private:
	struct FSkillData* CharacterSkillData = nullptr;

	void SetSkillQCooldownTimer();
	void SetSkillWCooldownTimer();
	void SetSkillECooldownTimer();
	void SetSkillRCooldownTimer();
	void SetDashCooldownTimer();
};


