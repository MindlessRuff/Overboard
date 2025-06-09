// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverboardGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Overboard/OverboardWidget.h"
#include "Overboard/Character/OverboardCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOverboardGameMode::AOverboardGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	RemainingTime = TimeLimit;

}

void AOverboardGameMode::BeginPlay()
{
	Super::BeginPlay();

	RemainingTime = TimeLimit;
	
	if (OverboardUIClass)
	{
		UOverboardWidget* OverboardUI = CreateWidget<UOverboardWidget>(GetWorld(), OverboardUIClass);
		if (OverboardUI)
		{
			OverboardUI->AddToViewport();
			OverboardWidget = OverboardUI;
		}
	}
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AOverboardGameMode::TickCountdown, 1.0f, true);
}

void AOverboardGameMode::EndPhase()
{
	// Save items, transition to raft level, etc.
	UGameplayStatics::OpenLevel(this, "Ocean");
}

void AOverboardGameMode::TickCountdown()
{
	RemainingTime--;

	if (RemainingTime <= 0)
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		EndPhase();
	}
	OverboardWidget->UpdateTimeDisplay(RemainingTime);
}

