// Copyright Epic Games, Inc. All Rights Reserved.

#include "PSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Particles/ParticleSystem.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "PSCharacterStatComponent.h"
#include <random>
#include "PSCharacterAnimInstance.h"
#include "IdentityProjectile.h"
#include "PSPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include "SpawnPoint.h"

APSCharacter::APSCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1100.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/PSCharacter/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	FString SkillDataPath = TEXT("/Game/Data/SkillData.SkillData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PS(*SkillDataPath);
	if (DT_PS.Succeeded()) {
		SkillDataTable = DT_PS.Object;
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CharacterStat = CreateDefaultSubobject<UPSCharacterStatComponent>(TEXT("CHARACTERSTAT")); // ĳ���� ���� ���̱�

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("ParticleSystem'/Game/FantasyMagic3/PS/PS_FireRing.PS_FireRing'"));
	if (Fire.Succeeded())
	{
		FireParticle = Fire.Object;
	}	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Wind(TEXT("ParticleSystem'/Game/FantasyMagic3/PS/PS_WindShield.PS_WindShield'"));
	if (Wind.Succeeded())
	{
		WindParticle = Wind.Object;
	}

	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(GetMesh());  // ��� ���������� ���� ��ƼŬ ������Ʈ
}

void APSCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void APSCharacter::BeginPlay()
{
	Super::BeginPlay();
	CharacterStat->Load();

	PSPC = Cast<APSPlayerController>(GetController()); // PS�÷��̾� ��Ʈ�ѷ� ��������

	GetWorldTimerManager().SetTimer(HPRegenHandle, this, &APSCharacter::SetCharacterHPRegen, 3.0f, true); // 3�ʸ��� ü����� �Լ� ����

	CharacterStat->OnHPIsZero.AddUObject(this, &APSCharacter::DeadPlayer);
	isDead = false;
}

void APSCharacter::DeadPlayer() // �÷��̾� ü�� 0�� �Ǿ� ����� ȣ��
{
	if (!isDead) {
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PSPC->DisableInput(PSPC); // �Է¸���
		UWidgetLayoutLibrary::RemoveAllWidgets(this); // ���� �����
		GetMesh()->PlayAnimation(DeathAnim, false);
		SpawnPoint = Cast<ASpawnPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnPoint::StaticClass()));
		SpawnPoint->TimeOut(); // ��������Ʈ ���ֱ�
		FTimerHandle myTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				PSPC->OnFailWidget(); // Fail UI ����
			}), 1.5f, false);
		isDead = true;
	}
}

void APSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PSAnim = Cast<UPSCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	PSAnim->OnSkillQ.AddUObject(this, &APSCharacter::SkillQ);
	PSAnim->OnSkillW.AddUObject(this, &APSCharacter::SkillW);
	PSAnim->OnSkillE.AddUObject(this, &APSCharacter::SkillE);
	PSAnim->OnSkillR.AddUObject(this, &APSCharacter::SkillR); // �ִ��ν��Ͻ��� ��Ƽ���̿� ����� ��������Ʈ �Լ�
	PSAnim->OnIdentity.AddUObject(this, &APSCharacter::Identity);

	CharacterStat->OnMaxMPChanged.AddUObject(this, &APSCharacter::CalcDamage);
	CharacterStat->OnAttackChanged.AddUObject(this, &APSCharacter::CalcDamage);
	CharacterStat->OnArmorChanged.AddUObject(this, &APSCharacter::CalcDamage);
	CharacterStat->OnSpeedChanged.AddUObject(this, &APSCharacter::CalcDamage);
	CharacterStat->OnSpeedChanged.AddUObject(this, &APSCharacter::SetCharacterSpeed);
	CharacterStat->OnDodgeChanged.AddUObject(this, &APSCharacter::CalcDamage);
	CharacterStat->OnLuckChanged.AddUObject(this, &APSCharacter::CalcDamage);
	CharacterStat->OnHPRegenChanged.AddUObject(this, &APSCharacter::CalcDamage);
	CharacterStat->OnLifestealChanged.AddUObject(this, &APSCharacter::CalcDamage);
	CharacterStat->OnAttackSpeedChanged.AddUObject(this, &APSCharacter::CalcDamage);
}

void APSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);
}

float APSCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (CharacterStat->GetDodge() >= 1) {
		int32 randNum = FMath::RandRange(1, 100);
		if (CharacterStat->GetDodge() > randNum) { // ȸ���� ��ġ�� ���� TakeDamage �������� �ʰ� ����
			return 1;
		}
	}
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	FinalDamage = FinalDamage - (FinalDamage * CharacterStat->GetArmor()); // ���� % ���

	CharacterStat->PSTakeDamage(FinalDamage);
	return FinalDamage;
}

void APSCharacter::TurnToMouseCursor()
{
	FHitResult Hit;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

	USkeletalMeshComponent* const MyPawn = GetMesh();  // �� Mesh ��������
	FVector ToTarget = Hit.Location - MyPawn->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, UKismetMathLibrary::ClampAxis(ToTarget.Rotation().Yaw), 0.f);

	SetActorRotation(LookAtRotation); // ���� ���콺 Ŀ���� ��ġ�� ĳ���� Rotate
}

FSkillData* APSCharacter::GetSkillData(int32 SkillNum)
{
	return SkillDataTable->FindRow<FSkillData>(*FString::FromInt(SkillNum), TEXT(""));
}

void APSCharacter::SkillQ(int32 UpgradeMode) {
	FVector StartLoc = GetMesh()->GetSocketLocation(TEXT("SkillR_Socket"));
	FVector TargetLoc = GetMesh()->GetSocketLocation(TEXT("SkillR_Socket"));

	TArray<AActor*> IngoreActors;
	IngoreActors.Add(this);

	TArray<FHitResult> HitRetArray;

	bool isHit = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), StartLoc, TargetLoc, 250.0f, TEXT("BlockAll"), false, IngoreActors, EDrawDebugTrace::Type::None, HitRetArray, true);
	if (isHit)
	{
		for (FHitResult Hit : HitRetArray)
		{
			if (AActor* Actor = Hit.Actor.Get())
			{
				if (Actor->ActorHasTag("Enemy")) {
					CharacterStat->SetMP(CharacterStat->GetMP() + 2.0f);  // ���� �������� 2 �߰�
					
					SetCharacterLifesteal(SkillQDmg * CharacterStat->GetLifesteal()); // ���� ��������ŭ ü��ȸ��
					UGameplayStatics::ApplyDamage(Actor, SkillQDmg, nullptr, this, NULL);
					FTimerHandle myTimerHandle, myTimerHandle2;
					GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([=]()
						{
							UGameplayStatics::ApplyDamage(Actor, SkillQDmg, nullptr, this, NULL);
							SetCharacterLifesteal(SkillQDmg * CharacterStat->GetLifesteal()); // ���� ��������ŭ ü��ȸ��
						}), 0.4f, false);
					GetWorld()->GetTimerManager().SetTimer(myTimerHandle2, FTimerDelegate::CreateLambda([=]()
						{
							UGameplayStatics::ApplyDamage(Actor, SkillQDmg, nullptr, this, NULL);
							SetCharacterLifesteal(SkillQDmg * CharacterStat->GetLifesteal()); // ���� ��������ŭ ü��ȸ��
						}), 0.8f, false);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Actor->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), FVector(2.0f, 2.0f, 2.0f));
				}
			}
		}
	}
}
void APSCharacter::SkillW(int32 UpgradeMode) {
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
				if (Actor->ActorHasTag("Enemy")) {
					CharacterStat->SetMP(CharacterStat->GetMP() + 2.0f);
					SetCharacterLifesteal(SkillWDmg * CharacterStat->GetLifesteal());
					UGameplayStatics::ApplyDamage(Actor, SkillWDmg, nullptr, this, NULL);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Actor->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), FVector(2.0f, 2.0f, 2.0f));
				}
			}
		}
	}
}
void APSCharacter::SkillE(int32 UpgradeMode) {
	FVector StartLoc = GetMesh()->GetSocketLocation(TEXT("SkillE_Socket"));
	FVector TargetLoc = GetMesh()->GetSocketLocation(TEXT("SkillE_Socket"));
	FRotator rot = GetActorRotation();

	TArray<AActor*> IngoreActors;
	IngoreActors.Add(this);

	TArray<FHitResult> HitRetArray;

	bool isHit = UKismetSystemLibrary::BoxTraceMultiByProfile(GetWorld(), StartLoc, TargetLoc, FVector(300.f,200.0f,50.0f) , rot, TEXT("BlockAll"), false, IngoreActors, EDrawDebugTrace::Type::None, HitRetArray, true);
	if (isHit)
	{
		for (FHitResult Hit : HitRetArray)
		{
			if (AActor* Actor = Hit.Actor.Get())
			{
				if (Actor->ActorHasTag("Enemy")) {
					CharacterStat->SetMP(CharacterStat->GetMP() + 2.0f);
					SetCharacterLifesteal(SkillEDmg * CharacterStat->GetLifesteal());
					UGameplayStatics::ApplyDamage(Actor, SkillEDmg, nullptr, this, NULL);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Actor->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), FVector(2.0f, 2.0f, 2.0f));
				}
			}
		}
	}
}
void APSCharacter::SkillR(int32 UpgradeMode) {
	FVector StartLoc = GetMesh()->GetSocketLocation(TEXT("SkillR_Socket"));
	FVector TargetLoc = GetMesh()->GetSocketLocation(TEXT("SkillR_Socket"));

	TArray<AActor*> IngoreActors;
	IngoreActors.Add(this);

	TArray<FHitResult> HitRetArray;

	bool isHit = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), StartLoc, TargetLoc, 300.f, TEXT("BlockAll"), false, IngoreActors, EDrawDebugTrace::Type::None, HitRetArray, true);
	if (isHit)
	{
		for (FHitResult Hit : HitRetArray)
		{
			if (AActor* Actor = Hit.Actor.Get())
			{
				if (Actor->ActorHasTag("Enemy")) {
					CharacterStat->SetMP(CharacterStat->GetMP() + 2.0f);
					SetCharacterLifesteal(SkillRDmg * CharacterStat->GetLifesteal());
					UGameplayStatics::ApplyDamage(Actor, SkillRDmg, nullptr, this, NULL);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Actor->GetActorLocation(),FRotator(0.0f,0.0f,0.0f),FVector(2.0f,2.0f,2.0f));
				}
			}
		}
	}
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			SkillRFollow(UpgradeMode); // 1�� �� R�� �ļӰ��� �Լ� ����
		}), 1.0f, false);
}
void APSCharacter::SkillRFollow(int32 UpgradeMode) {
	SetCanBeDamaged(true); // R �����ð� ����

	FVector StartLoc = GetMesh()->GetSocketLocation(TEXT("SkillR_Socket"));
	FVector TargetLoc = GetMesh()->GetSocketLocation(TEXT("SkillR_Socket"));

	TArray<AActor*> IngoreActors;
	IngoreActors.Add(this);

	TArray<FHitResult> HitRetArray;

	bool isHit = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), StartLoc, TargetLoc, 350.f, TEXT("BlockAll"), false, IngoreActors, EDrawDebugTrace::Type::None, HitRetArray, true);
	if (isHit)
	{
		for (FHitResult Hit : HitRetArray)
		{
			if (AActor* Actor = Hit.Actor.Get())
			{
				if (Actor->ActorHasTag("Enemy")) {
					SetCharacterLifesteal(SkillRDmg * CharacterStat->GetLifesteal());
					UGameplayStatics::ApplyDamage(Actor, SkillRDmg, nullptr, this, NULL);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Actor->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), FVector(2.0f, 2.0f, 2.0f));
				}
			}
		}
	}
}
void APSCharacter::CalcDamage()
{
	int32 Mode = CharacterStat->GetUpgradeMode(); // ���� ��ų ���� ��Ȳ�� ���� ������ ����
	switch (Mode)
	{
	case 0: // ���� x
		SkillQDmg = CharacterStat->GetAttack() * 0.3f;
		SkillWDmg = CharacterStat->GetAttack() * 0.8f;
		SkillEDmg = CharacterStat->GetAttack() * 0.7f;
		SkillRDmg = CharacterStat->GetAttack() * 0.6f;
		IdentityDmg = CharacterStat->GetAttack() * 1.f;
		break;
	case 1: // MaxHP ��� ������ ���
		SkillQDmg = CharacterStat->GetAttack() * 0.3f * ((CharacterStat->GetHP() * 0.004f) + 1);
		SkillWDmg = CharacterStat->GetAttack() * 0.8f * ((CharacterStat->GetHP() * 0.004f) + 1);
		SkillEDmg = CharacterStat->GetAttack() * 0.7f * ((CharacterStat->GetHP() * 0.004f) + 1);
		SkillRDmg = CharacterStat->GetAttack() * 0.6f * ((CharacterStat->GetHP() * 0.004f) + 1);
		IdentityDmg = CharacterStat->GetAttack() * 1.f * ((CharacterStat->GetHP() * 0.004f) + 1);
		break;
	case 2: // Attack ��� ������ ���
		SkillQDmg = CharacterStat->GetAttack() * 0.3f * ((CharacterStat->GetAttack() * 0.005f) + 1);
		SkillWDmg = CharacterStat->GetAttack() * 0.8f * ((CharacterStat->GetAttack() * 0.005f) + 1);
		SkillEDmg = CharacterStat->GetAttack() * 0.7f * ((CharacterStat->GetAttack() * 0.005f) + 1);
		SkillRDmg = CharacterStat->GetAttack() * 0.6f * ((CharacterStat->GetAttack() * 0.005f) + 1);
		IdentityDmg = CharacterStat->GetAttack() * 1.f * ((CharacterStat->GetAttack() * 0.005f) + 1);
		break;
	case 3: // Armor ��� ������ ���
		SkillQDmg = CharacterStat->GetAttack() * 0.3f * ((CharacterStat->GetArmor()) + 1);
		SkillWDmg = CharacterStat->GetAttack() * 0.8f * ((CharacterStat->GetArmor()) + 1);
		SkillEDmg = CharacterStat->GetAttack() * 0.7f * ((CharacterStat->GetArmor()) + 1);
		SkillRDmg = CharacterStat->GetAttack() * 0.6f * ((CharacterStat->GetArmor()) + 1);
		IdentityDmg = CharacterStat->GetAttack() * 1.f * ((CharacterStat->GetArmor()) + 1);
		break;
	case 4: // Speed ��� ������ ���
		SkillQDmg = CharacterStat->GetAttack() * 0.3f * ((CharacterStat->GetSpeed() * 0.0055f) + 1);
		SkillWDmg = CharacterStat->GetAttack() * 0.8f * ((CharacterStat->GetSpeed() * 0.0055f) + 1);
		SkillEDmg = CharacterStat->GetAttack() * 0.7f * ((CharacterStat->GetSpeed() * 0.0055f) + 1);
		SkillRDmg = CharacterStat->GetAttack() * 0.6f * ((CharacterStat->GetSpeed() * 0.0055f) + 1);
		IdentityDmg = CharacterStat->GetAttack() * 1.f * ((CharacterStat->GetSpeed() * 0.0055f) + 1);
		break;
	case 5: // Dodge ��� ������ ���
		SkillQDmg = CharacterStat->GetAttack() * 0.3f * ((CharacterStat->GetDodge() * 0.01f) + 1);
		SkillWDmg = CharacterStat->GetAttack() * 0.8f * ((CharacterStat->GetDodge() * 0.01f) + 1);
		SkillEDmg = CharacterStat->GetAttack() * 0.7f * ((CharacterStat->GetDodge() * 0.01f) + 1);
		SkillRDmg = CharacterStat->GetAttack() * 0.6f * ((CharacterStat->GetDodge() * 0.01f) + 1);
		IdentityDmg = CharacterStat->GetAttack() * 1.f * ((CharacterStat->GetDodge() * 0.01f) + 1);
		break;
	case 6: // Luck ��� ������ ���
		SkillQDmg = CharacterStat->GetAttack() * 0.3f * (CharacterStat->GetLuck()*0.02+1);
		SkillWDmg = CharacterStat->GetAttack() * 0.8f * (CharacterStat->GetLuck() * 0.02 + 1);
		SkillEDmg = CharacterStat->GetAttack() * 0.7f * (CharacterStat->GetLuck() * 0.02 + 1);
		SkillRDmg = CharacterStat->GetAttack() * 0.6f * (CharacterStat->GetLuck() * 0.02 + 1);
		IdentityDmg = CharacterStat->GetAttack() * 1.f * (CharacterStat->GetLuck() * 0.02 + 1);
		break;
	case 7: // HPRegen ��� ������ ���
		SkillQDmg = CharacterStat->GetAttack() * 0.3f * (CharacterStat->GetHPRegen() * 0.02 + 1);
		SkillWDmg = CharacterStat->GetAttack() * 0.8f * (CharacterStat->GetHPRegen() * 0.02 + 1);
		SkillEDmg = CharacterStat->GetAttack() * 0.7f * (CharacterStat->GetHPRegen() * 0.02 + 1);
		SkillRDmg = CharacterStat->GetAttack() * 0.6f * (CharacterStat->GetHPRegen() * 0.02 + 1);
		IdentityDmg = CharacterStat->GetAttack() * 1.f * (CharacterStat->GetHPRegen() * 0.02 + 1);
		break;
	case 8: // Lifesteal ��� ������ ���
		SkillQDmg = CharacterStat->GetAttack() * 0.3f * (CharacterStat->GetLifesteal() * 10.f + 1);
		SkillWDmg = CharacterStat->GetAttack() * 0.8f * (CharacterStat->GetLifesteal() * 10.f + 1);
		SkillEDmg = CharacterStat->GetAttack() * 0.7f * (CharacterStat->GetLifesteal() * 10.f + 1);
		SkillRDmg = CharacterStat->GetAttack() * 0.6f * (CharacterStat->GetLifesteal() * 10.f + 1);
		IdentityDmg = CharacterStat->GetAttack() * 1.f * (CharacterStat->GetLifesteal() * 10.f + 1);
		break;
	case 9: // AttackSpeed ��� ������ ���
		SkillQDmg = CharacterStat->GetAttack() * 0.3f * ((CharacterStat->GetAttackSpeed()) + 1);
		SkillWDmg = CharacterStat->GetAttack() * 0.8f * ((CharacterStat->GetAttackSpeed()) + 1);
		SkillEDmg = CharacterStat->GetAttack() * 0.7f * ((CharacterStat->GetAttackSpeed()) + 1);
		SkillRDmg = CharacterStat->GetAttack() * 0.6f * ((CharacterStat->GetAttackSpeed()) + 1);
		IdentityDmg = CharacterStat->GetAttack() * 1.f * ((CharacterStat->GetAttackSpeed()) + 1);
		break;
	default:
		break;
	}
}
void APSCharacter::Identity() { // ���̵�ƼƼ �������� ���� ���� ����
	FActorSpawnParameters params; 
	params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AIdentityProjectile>(spawn_obj, GetActorLocation(), GetActorRotation(), params);
}
float APSCharacter::getIdentityRatio() {
	return PSPC->IdentityRatio * IdentityDmg; // ��Ʈ�ѷ����� ���� ���̵�ƼƼ ����� ���� ��ȯ
}

void APSCharacter::Dash(float DashDistance)
{
	if (!OnDash) {
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		LaunchCharacter(GetActorForwardVector() * DashDistance, true, false);
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // ECC_GameTraceChannel1 = Enemy�� ����ϴ� ä��
		OnDash = true;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &APSCharacter::StopDashing, 0.1f, false); // 0.1�ʵ��� ĳ���� Launch
	}
}
void APSCharacter::StopDashing() {
	GetCharacterMovement()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2); // ECC_GameTraceChannel2 = Player (�ݸ��� Ȱ��ȭ)
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APSCharacter::ResetDash, 0.2f, false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}
void APSCharacter::ResetDash() {
	OnDash = false;
}

void APSCharacter::SetCharacterSpeed() {
	GetCharacterMovement()->MaxWalkSpeed += CharacterStat->GetSpeed(); // ĳ���� �ӵ� ���Ҷ����� ����
}

void APSCharacter::SetCharacterHPRegen() {
	CharacterStat->SetHP(CharacterStat->GetCurrentHP() + CharacterStat->GetHPRegen()); // HPRegen��ŭ SetHP
}

void APSCharacter::SetCharacterLifesteal(float dmg) {
	CharacterStat->SetHP(CharacterStat->GetCurrentHP() + dmg); // dmg��ŭ SetHP
}

void APSCharacter::DropHealing() {
	CharacterStat->SetHP(CharacterStat->GetCurrentHP() + (CharacterStat->GetHP()*0.25f)); // �ִ�ü���� 25% ȸ��
}

void APSCharacter::DropSpeedUp()
{
	OurParticleSystem->SetTemplate(WindParticle); // ��ƼŬ Ȱ��ȭ

	float NewSpeed = GetCharacterMovement()->MaxWalkSpeed * 0.15f; // �̵��ӵ� 15% ����
	GetCharacterMovement()->MaxWalkSpeed += NewSpeed;

	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([=]() // ���ٽ� &->���۷��� ĸ��, = �� �ؾ���
		{
			OurParticleSystem->SetTemplate(NULL); // ��ƼŬ ��Ȱ��ȭ
			GetCharacterMovement()->MaxWalkSpeed -= NewSpeed;
		}), 5.0f, false);
}

void APSCharacter::DropAttackUp()
{
	OurParticleSystem->SetTemplate(FireParticle);

	float NewAttack = CharacterStat->GetAttack() * 0.2f; // ���ݷ� 20% ����
	CharacterStat->SetAttack(NewAttack);

	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([=]() // ���ٽ� &->���۷��� ĸ��, = �� �ؾ���
		{
			OurParticleSystem->SetTemplate(NULL);
			CharacterStat->SetAttack(-NewAttack);
		}), 5.0f, false);
}

/**
*	
*/
void APSCharacter::DropIdentity()
{
	CharacterStat->SetMP(CharacterStat->GetMP() + 50.0f); // ���� 50 ȸ��
}

void APSCharacter::SaveStageName(FString StageName)
{
	CharacterStat->StageName = StageName;
	CharacterStat->Save(); // �������� ����� ����BP���� �������� �̸� �Ѱܹް� ����
}
