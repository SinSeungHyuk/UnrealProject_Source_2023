// Copyright Epic Games, Inc. All Rights Reserved.

#include "PSPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "PSCharacterAnimInstance.h"
#include "PSCharacterStatComponent.h"
#include "PSCharacter.h"
#include "Engine/World.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

APSPlayerController::APSPlayerController()
{
	bShowMouseCursor = true;
	bStartMoveToMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	static ConstructorHelpers::FClassFinder<UPSCharacterHPBarWidget> HPBarWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/PSCharacterHPBar.PSCharacterHPBar_C'"));
	if (HPBarWidgetAsset.Succeeded())
		HPBarWidgetClass = HPBarWidgetAsset.Class;

	static ConstructorHelpers::FClassFinder<UPSLevelUPWidget> UpgradeWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/LevelUpUI.LevelUpUI_C'"));
	if (UpgradeWidgetAsset.Succeeded())
		UpgradeWidgetClass = UpgradeWidgetAsset.Class;

	static ConstructorHelpers::FClassFinder<UPSShopWidget> ShopWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/ShopUI.ShopUI_C'"));
	if (ShopWidgetAsset.Succeeded())
		ShopWidgetClass = ShopWidgetAsset.Class;

	static ConstructorHelpers::FClassFinder<UPSSkillUpgradeWidget> SkillUpgradeWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/SkillUpgradeUI.SkillUpgradeUI_C'"));
	if (SkillUpgradeWidgetAsset.Succeeded())
		SkillUpgradeWidgetClass = SkillUpgradeWidgetAsset.Class;

	static ConstructorHelpers::FClassFinder<UPSPauseWidget> PauseWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/PauseUI.PauseUI_C'"));
	if (PauseWidgetAsset.Succeeded())
		PauseWidgetClass = PauseWidgetAsset.Class;

	static ConstructorHelpers::FClassFinder<UPSClearWidget> ClearWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/ClearUI.ClearUI_C'"));
	if (ClearWidgetAsset.Succeeded())
		ClearWidgetClass = ClearWidgetAsset.Class;

	static ConstructorHelpers::FClassFinder<UPSClearWidget> FailWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/FailUI.FailUI_C'"));
	if (FailWidgetAsset.Succeeded())
		FailWidgetClass = FailWidgetAsset.Class;
	// �ʿ��� UI ������ �����ڿ��� ����
}

void APSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	APSCharacter* YourCharacter = Cast<APSCharacter>(GetCharacter());

	if (IsValid(HPBarWidgetClass))
	{
		HPBarWidget = Cast<UPSCharacterHPBarWidget>(CreateWidget(GetWorld(), HPBarWidgetClass));

		if (IsValid(HPBarWidget))
		{
			// ������ ����Ʈ�� ���� �Լ�
			HPBarWidget->AddToViewport(); // �÷��̾� ĳ������ ���,�ð�,ü��,��ٿ� �� UI
			HPBarWidget->BindCharacterStat(YourCharacter->CharacterStat);
		}
	}

	CharacterSkillData = YourCharacter->GetSkillData(0); // ��ų ������ �������� 
}

void APSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bStartMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void APSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APSPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APSPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("SkillQ", IE_Released, this, &APSPlayerController::SkillQ);
	InputComponent->BindAction("SkillW", IE_Released, this, &APSPlayerController::SkillW);
	InputComponent->BindAction("SkillE", IE_Released, this, &APSPlayerController::SkillE);
	InputComponent->BindAction("SkillR", IE_Released, this, &APSPlayerController::SkillR);
	InputComponent->BindAction("Dash", IE_Released, this, &APSPlayerController::Dash);
	InputComponent->BindAction("Identity", IE_Released, this, &APSPlayerController::Identity);

	InputComponent->BindAction("Pause", IE_Released, this, &APSPlayerController::OnPauseWidget);
}

void APSPlayerController::MoveToMouseCursor()
{
	if (!bMoveToMouseCursor && bStartMoveToMouseCursor) {
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
			bMoveToMouseCursor = true;
		}
	}
}

void APSPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void APSPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	//bMoveToMouseCursor = true;
	bStartMoveToMouseCursor = true;
	MoveToMouseCursor();
}

void APSPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bStartMoveToMouseCursor = false;
	bMoveToMouseCursor = false;
}

// ���콺 Ŭ���� �ҋ����� �̵��ϸ� �� �����°��� �̵� X


void APSPlayerController::SkillQ()
{
	if (!GetWorldTimerManager().IsTimerActive(SkillQTimer)) {
		// 1�ʸ��� ����� ��ų�� ��ٿ� ���� �Լ��� ������ (���� �ð��� ������ ����Ͽ� UI�� ��ϵ� ��������Ʈ �Լ��� ���� ǥ��)
		GetWorld()->GetTimerManager().SetTimer(SkillQCooldownTimer, this, &APSPlayerController::SetSkillQCooldownTimer, 1.0f, true);

		APSCharacter* YourCharacter = Cast<APSCharacter>(GetCharacter());
		YourCharacter->TurnToMouseCursor(); // ��ų�� �Է��� ���� ���콺 Ŀ���� ��ġ�� ĳ���� ȸ��
		YourCharacter->PSAnim->PlaySkillMontage(0, YourCharacter->CharacterStat->GetAttackSpeed());

		// ��ų �������� ��ٿŭ �� �Լ��� ���� ������
		GetWorldTimerManager().SetTimer(SkillQTimer, this, &APSPlayerController::SkillQ, CharacterSkillData->SkillQCooldown);
	}
}
void APSPlayerController::SkillW()
{
	if (!GetWorldTimerManager().IsTimerActive(SkillWTimer)) {
		GetWorld()->GetTimerManager().SetTimer(SkillWCooldownTimer, this, &APSPlayerController::SetSkillWCooldownTimer, 1.0f, true);
		APSCharacter* YourCharacter = Cast<APSCharacter>(GetCharacter());
		YourCharacter->TurnToMouseCursor();
		YourCharacter->PSAnim->PlaySkillMontage(1, YourCharacter->CharacterStat->GetAttackSpeed());
		GetWorldTimerManager().SetTimer(SkillWTimer, this, &APSPlayerController::SkillW, CharacterSkillData->SkillWCooldown);
	}
}
void APSPlayerController::SkillE()
{
	if (!GetWorldTimerManager().IsTimerActive(SkillETimer)) {
		GetWorld()->GetTimerManager().SetTimer(SkillECooldownTimer, this, &APSPlayerController::SetSkillECooldownTimer, 1.0f, true);
		APSCharacter* YourCharacter = Cast<APSCharacter>(GetCharacter());
		YourCharacter->TurnToMouseCursor();
		YourCharacter->PSAnim->PlaySkillMontage(2, YourCharacter->CharacterStat->GetAttackSpeed());
		GetWorldTimerManager().SetTimer(SkillETimer, this, &APSPlayerController::SkillE, CharacterSkillData->SkillECooldown); 
	}
}
void APSPlayerController::SkillR()
{
	if (!GetWorldTimerManager().IsTimerActive(SkillRTimer)) {
		GetWorld()->GetTimerManager().SetTimer(SkillRCooldownTimer, this, &APSPlayerController::SetSkillRCooldownTimer, 1.0f, true);
		APSCharacter* YourCharacter = Cast<APSCharacter>(GetCharacter());
		YourCharacter->TurnToMouseCursor();
		YourCharacter->Dash(3000.0f);
		YourCharacter->SetCanBeDamaged(false);
		YourCharacter->PSAnim->PlaySkillMontage(3, YourCharacter->CharacterStat->GetAttackSpeed());
		GetWorldTimerManager().SetTimer(SkillRTimer, this, &APSPlayerController::SkillR, CharacterSkillData->SkillRCooldown);
	}
}
void APSPlayerController::Dash()
{
	if (!GetWorldTimerManager().IsTimerActive(DashTimer)) {
		GetWorld()->GetTimerManager().SetTimer(DashCooldownTimer, this, &APSPlayerController::SetDashCooldownTimer, 1.0f, true);
		APSCharacter* YourCharacter = Cast<APSCharacter>(GetCharacter());
		YourCharacter->TurnToMouseCursor();
		YourCharacter->Dash(3000.0f); // 3000 �Ÿ���ŭ Dash
		YourCharacter->PSAnim->PlaySkillMontage(4,0.0f);
		GetWorldTimerManager().SetTimer(DashTimer, this, &APSPlayerController::Dash, CharacterSkillData->DashCooldown);
	}
}
void APSPlayerController::Identity()
{
	APSCharacter* YourCharacter = Cast<APSCharacter>(GetCharacter());
	if (YourCharacter->CharacterStat->GetMP() >= 50.0f) { // ���� MP�� 50 �̻��϶��� ���
		YourCharacter->TurnToMouseCursor();
		IdentityRatio = YourCharacter->CharacterStat->getIdentityRatio();
		YourCharacter->CharacterStat->SetMP(0); // ���� ��� MP �Ҹ�
		YourCharacter->PSAnim->PlaySkillMontage(5, YourCharacter->CharacterStat->GetAttackSpeed()); // ���̵�ƼƼ ��� �ִϸ��̼�
	}
}

void APSPlayerController::OnLevelUPWidget() // ������ UI ����
{
	if (IsValid(UpgradeWidgetClass))
	{
		UpgradeWidget = Cast<UPSLevelUPWidget>(CreateWidget(GetWorld(), UpgradeWidgetClass));

		if (IsValid(UpgradeWidget))
		{
			APSCharacter* CharacterSt = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			UpgradeWidget->BindCharacterStat(CharacterSt->CharacterStat);
			UpgradeWidget->AddToViewport();
		}
	}
}

void APSPlayerController::OnShopWidget() // ���� UI ����
{
	if (IsValid(ShopWidgetClass))
	{
		ShopWidget = Cast<UPSShopWidget>(CreateWidget(GetWorld(), ShopWidgetClass));

		if (IsValid(ShopWidget))
		{
			APSCharacter* CharacterSt = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			ShopWidget->BindCharacterStat(CharacterSt->CharacterStat);
			ShopWidget->AddToViewport();
		}
	}
}

void APSPlayerController::OnSkillUpgradeWidget(int32 UpgradeMode) // ��ų���׷��̵� UI ����
{
	if (IsValid(SkillUpgradeWidgetClass))
	{
		APSCharacter* CharacterSt = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (CharacterSt->CharacterStat->GetUpgradeMode() == 0) {
			SkillUpgradeWidget = Cast<UPSSkillUpgradeWidget>(CreateWidget(GetWorld(), SkillUpgradeWidgetClass));

			if (IsValid(SkillUpgradeWidget))
			{
				SkillUpgradeWidget->BindCharacterStat(UpgradeMode);
				SkillUpgradeWidget->AddToViewport();
			}
		}
	}
}

void APSPlayerController::OnPauseWidget() // �Ͻ����� UI ����
{
	if (IsValid(PauseWidgetClass))
	{
		PauseWidget = Cast<UPSPauseWidget>(CreateWidget(GetWorld(), PauseWidgetClass));

		if (IsValid(PauseWidget))
		{
			APSCharacter* CharacterSt = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			PauseWidget->BindCharacterStat(CharacterSt->CharacterStat);
			PauseWidget->AddToViewport();
		}
	}
}

void APSPlayerController::OnClearWidget() // Ŭ���� UI ����
{
	if (IsValid(ClearWidgetClass))
	{
		ClearWidget = Cast<UPSClearWidget>(CreateWidget(GetWorld(), ClearWidgetClass));

		if (IsValid(ClearWidget))
		{
			APSCharacter* CharacterSt = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			ClearWidget->BindCharacterStat(CharacterSt->CharacterStat);
			ClearWidget->AddToViewport();
		}
	}
}

void APSPlayerController::OnFailWidget() // ���� UI ����
{
	if (IsValid(FailWidgetClass))
	{
		FailWidget = Cast<UPSClearWidget>(CreateWidget(GetWorld(), FailWidgetClass));

		if (IsValid(FailWidget))
		{
			APSCharacter* CharacterSt = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			FailWidget->BindCharacterStat(CharacterSt->CharacterStat);
			FailWidget->AddToViewport();
		}
	}
}

void APSPlayerController::SetSkillQCooldownTimer()
{
	// ����� ��ų�� Ÿ�̸Ӱ� 0�� �̻� ��������
	if (GetWorldTimerManager().GetTimerElapsed(SkillQTimer) >= 0)
	{
		// ����� ��ų Ÿ�̸��� ���� �ð��� ��ų�������� ��ٿ��� ����Ͽ� ���� �ð��� ������ ��ȯ�Ͽ� ��������Ʈ ȣ��
		float remainingTime = GetWorldTimerManager().GetTimerElapsed(SkillQTimer) / CharacterSkillData->SkillQCooldown;
		OnSkillQCooldown.Broadcast(remainingTime);
	} else 	GetWorld()->GetTimerManager().ClearTimer(SkillQCooldownTimer); // ���� �ð��� ������� (��ٿ��� �ƴҰ��) Ÿ�̸� ����
}

void APSPlayerController::SetSkillWCooldownTimer()
{
	if (GetWorldTimerManager().GetTimerElapsed(SkillWTimer) >= 0)
	{
		float remainingTime = GetWorldTimerManager().GetTimerElapsed(SkillWTimer) / CharacterSkillData->SkillWCooldown;
		OnSkillWCooldown.Broadcast(remainingTime);
	}
	else 	GetWorld()->GetTimerManager().ClearTimer(SkillWCooldownTimer);
}

void APSPlayerController::SetSkillECooldownTimer()
{
	if (GetWorldTimerManager().GetTimerElapsed(SkillETimer) >= 0)
	{
		float remainingTime = GetWorldTimerManager().GetTimerElapsed(SkillETimer) / CharacterSkillData->SkillECooldown;
		OnSkillECooldown.Broadcast(remainingTime);
	}
	else 	GetWorld()->GetTimerManager().ClearTimer(SkillECooldownTimer);
}

void APSPlayerController::SetSkillRCooldownTimer()
{
	if (GetWorldTimerManager().GetTimerElapsed(SkillRTimer) >= 0)
	{
		float remainingTime = GetWorldTimerManager().GetTimerElapsed(SkillRTimer) / CharacterSkillData->SkillRCooldown;
		OnSkillRCooldown.Broadcast(remainingTime);
	}
	else 	GetWorld()->GetTimerManager().ClearTimer(SkillRCooldownTimer);
}

void APSPlayerController::SetDashCooldownTimer()
{
	if (GetWorldTimerManager().GetTimerElapsed(DashTimer) >= 0)
	{
		float remainingTime = GetWorldTimerManager().GetTimerElapsed(DashTimer) / CharacterSkillData->DashCooldown;
		OnDashCooldown.Broadcast(remainingTime);
	}
	else 	GetWorld()->GetTimerManager().ClearTimer(DashCooldownTimer);
}