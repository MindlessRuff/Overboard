// Fill out your copyright notice in the Description page of Project Settings.


#include "RaftGameState.h"

#include "JsonObjectConverter.h"

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
	FString FilePath = FPaths::ProjectContentDir() + "Data/IntroTexts.json";
	FString JsonContent;

	if (FFileHelper::LoadFileToString(JsonContent, *FilePath))
	{
		FJsonObjectConverter::JsonArrayStringToUStruct(JsonContent, &LoadedEntries, 0, 0);
		for (const FRaftDayTextEntry& Entry : LoadedEntries)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Day %d | Text: %s"),
			// 	Entry.Day,
			// 	*Entry.Text);
		}
	}
	StartIntro();
}

void ARaftGameState::StartIntro()
{
	Text_Event = FText::FromString(LoadedEntries[0].Text);
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
	if (CurrentDay)
	{
		int32 RandomIndex = FMath::RandRange(0, LoadedEntries.Num() - 1);
		Text_Event = FText::FromString(LoadedEntries[RandomIndex].Text);
	}
}

void ARaftGameState::CheckGameOver()
{
	if (Food <= 0 || Water <= 0 || Sanity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over!"));
		// Trigger end screen or restart
	}
}
