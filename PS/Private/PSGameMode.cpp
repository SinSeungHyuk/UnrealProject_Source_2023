// Copyright Epic Games, Inc. All Rights Reserved.

#include "PSGameMode.h"
#include "PSPlayerController.h"
#include "PSCharacter.h"
#include "UObject/ConstructorHelpers.h"

APSGameMode::APSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/PSCharacter/Blueprints/PSCharacterBP"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}