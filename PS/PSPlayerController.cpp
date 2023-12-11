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
	// 필요한 UI 위젯들 생성자에서 관리
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
			// 위젯을 뷰포트에 띄우는 함수
			HPBarWidget->AddToViewport(); // 플레이어 캐릭터의 골드,시간,체력,쿨다운 등 UI
			HPBarWidget->BindCharacterStat(YourCharacter->CharacterStat);
		}
	}

	CharacterSkillData = YourCharacter->GetSkillData(0); // 스킬 데이터 가져오기 
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

// 마우스 클릭을 할떄에만 이동하며 꾹 누르는것은 이동 X


void APSPlayerController::SkillQ()
{
	if (!GetWorldTimerManager().IsTimerActive(SkillQTimer)) {
		// 1초마다 사용한 스킬의 쿨다운 갱신 함수를 보내줌 (남은 시간의 비율을 계산하여 UI에 등록된 델리게이트 함수를 통해 표시)
		GetWorld()->GetTimerManager().SetTimer(SkillQCooldownTimer, this, &APSPlayerController::SetSkillQCooldownTimer, 1.0f, true);

		APSCharacter* YourCharacter = Cast<APSCharacter>(GetCharacter());
		YourCharacter->TurnToMouseCursor(); // 스킬을 입력한 순간 마우스 커서의 위치로 캐릭터 회전
		YourCharacter->PSAnim->PlaySkillMontage(0, YourCharacter->CharacterStat->GetAttackSpeed());

		// 스킬 데이터의 쿨다운만큼 이 함수의 실행 딜레이
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
		YourCharacter->Dash(3000.0f); // 3000 거리만큼 Dash
		YourCharacter->PSAnim->PlaySkillMontage(4,0.0f);
		GetWorldTimerManager().SetTimer(DashTimer, this, &APSPlayerController::Dash, CharacterSkillData->DashCooldown);
	}
}
void APSPlayerController::Identity()
{
	APSCharacter* YourCharacter = Cast<APSCharacter>(GetCharacter());
	if (YourCharacter->CharacterStat->GetMP() >= 50.0f) { // 현재 MP가 50 이상일때만 사용
		YourCharacter->TurnToMouseCursor();
		IdentityRatio = YourCharacter->CharacterStat->getIdentityRatio();
		YourCharacter->CharacterStat->SetMP(0); // 사용시 모든 MP 소모
		YourCharacter->PSAnim->PlaySkillMontage(5, YourCharacter->CharacterStat->GetAttackSpeed()); // 아이덴티티 사용 애니메이션
	}
}

void APSPlayerController::OnLevelUPWidget() // 레벨업 UI 생성
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

void APSPlayerController::OnShopWidget() // 상점 UI 생성
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

void APSPlayerController::OnSkillUpgradeWidget(int32 UpgradeMode) // 스킬업그레이드 UI 생성
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

void APSPlayerController::OnPauseWidget() // 일시정지 UI 생성
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

void APSPlayerController::OnClearWidget() // 클리어 UI 생성
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

void APSPlayerController::OnFailWidget() // 실패 UI 생성
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
	// 사용한 스킬의 타이머가 0초 이상 남았을때
	if (GetWorldTimerManager().GetTimerElapsed(SkillQTimer) >= 0)
	{
		// 사용한 스킬 타이머의 남은 시간과 스킬데이터의 쿨다운을 계산하여 남은 시간의 비율을 반환하여 델리게이트 호출
		float remainingTime = GetWorldTimerManager().GetTimerElapsed(SkillQTimer) / CharacterSkillData->SkillQCooldown;
		OnSkillQCooldown.Broadcast(remainingTime);
	} else 	GetWorld()->GetTimerManager().ClearTimer(SkillQCooldownTimer); // 남은 시간이 없을경우 (쿨다운이 아닐경우) 타이머 해제
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