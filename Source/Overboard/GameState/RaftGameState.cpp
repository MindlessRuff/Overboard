// Fill out your copyright notice in the Description page of Project Settings.


#include "RaftGameState.h"

ARaftGameState::ARaftGameState()
{
	CurrentDay = 1;
	Food = 100;
	Water = 100;
	Sanity = 100;
}

void ARaftGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ARaftGameState::AdvanceDay()
{
	++CurrentDay;
	ApplyDailyDecay();
	TriggerRandomEvent();
	CheckGameOver();
}

void ARaftGameState::ApplyDailyDecay()
{
	Food = FMath::Max(0, Food - 10);
	Water = FMath::Max(0, Water - 15);
	Sanity = FMath::Max(0, Sanity - 5);
}

void ARaftGameState::TriggerRandomEvent()
{
	// Placeholder for future random event manager
}

void ARaftGameState::CheckGameOver()
{
	if (Food <= 0 || Water <= 0 || Sanity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over!"));
		// Trigger end screen or restart
	}
}
