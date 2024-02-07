// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearWidgetCreateActor.h"
#include "PSCharacter.h"
#include "PSPlayerController.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
AClearWidgetCreateActor::AClearWidgetCreateActor()
{

}

// Called when the game starts or when spawned
void AClearWidgetCreateActor::BeginPlay()
{
	Super::BeginPlay();
	

	GetWorld()->GetTimerManager().SetTimer(WidgetHandle, FTimerDelegate::CreateLambda([&]()
		{
			auto PlayerController = Cast<APSPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

			PlayerController->OnClearWidget();
		}), 7.0f, false);
}

