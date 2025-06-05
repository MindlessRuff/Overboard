// Fill out your copyright notice in the Description page of Project Settings.

#include "RaftGameMode.h"
#include "RaftGameState.h"
#include "RaftSurvivalWidget.h"
#include "Kismet/GameplayStatics.h"

ARaftGameMode::ARaftGameMode()
{
	DefaultPawnClass = nullptr;
	HUDClass = URaftSurvivalWidget::StaticClass();
	GameStateClass = ARaftGameState::StaticClass();
}

void ARaftGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (RaftUIClass)
	{
		URaftSurvivalWidget* RaftUI = CreateWidget<URaftSurvivalWidget>(GetWorld(), RaftUIClass);
		if (RaftUI)
		{
			RaftUI->AddToViewport();  // Adds the UI to the screen

			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}
