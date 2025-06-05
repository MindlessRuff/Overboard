// Fill out your copyright notice in the Description page of Project Settings.


#include "RaftSurvivalWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "RaftGameState.h"

void URaftSurvivalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button click
	if (Button_AdvanceDay)
	{
		Button_AdvanceDay->OnClicked.AddDynamic(this, &URaftSurvivalWidget::OnAdvanceDayClicked);
	}

	UpdateSurvivalStats();
}

void URaftSurvivalWidget::OnAdvanceDayClicked()
{
	if (ARaftGameState* GS = GetWorld()->GetGameState<ARaftGameState>())
	{
		GS->AdvanceDay();
		UpdateSurvivalStats();
	}
}

void URaftSurvivalWidget::UpdateSurvivalStats()
{
	if (ARaftGameState* GS = GetWorld()->GetGameState<ARaftGameState>())
	{
		if (Text_DayLabel)
			Text_DayLabel->SetText(FText::FromString(FString::Printf(TEXT("Day: %d"), GS->CurrentDay)));
		if (Text_Food)
			Text_Food->SetText(FText::FromString(FString::Printf(TEXT("Food: %d"), GS->Food)));
		if (Text_Water)
			Text_Water->SetText(FText::FromString(FString::Printf(TEXT("Water: %d"), GS->Water)));
		if (Text_Sanity)
			Text_Sanity->SetText(FText::FromString(FString::Printf(TEXT("Sanity: %d"), GS->Sanity)));
		if (Text_Button_AdvanceDay)
			Text_Button_AdvanceDay->SetText(FText::FromString(FString::Printf(TEXT("Next Day"))));
	}
}