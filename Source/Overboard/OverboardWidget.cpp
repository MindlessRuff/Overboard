// Fill out your copyright notice in the Description page of Project Settings.


#include "OverboardWidget.h"
#include "Components/TextBlock.h"

void UOverboardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOverboardWidget::UpdateTimeDisplay(float TimeRemaining)
{
	if (TimeRemainingText)
	{
		int32 Minutes = FMath::FloorToInt(TimeRemaining / 60);
		int32 Seconds = FMath::FloorToInt(FMath::Fmod(TimeRemaining, 60));
		FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TimeRemainingText->SetText(FText::FromString(TimeString));
	}
}