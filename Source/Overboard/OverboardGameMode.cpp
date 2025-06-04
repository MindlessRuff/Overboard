// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverboardGameMode.h"
#include "OverboardCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOverboardGameMode::AOverboardGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
