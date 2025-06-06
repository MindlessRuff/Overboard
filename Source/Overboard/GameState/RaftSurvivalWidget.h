// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaftSurvivalWidget.generated.h"

/**
 * 
 */

class UTextBlock;
class UButton;

UCLASS()
class OVERBOARD_API URaftSurvivalWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateSurvivalStats();
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Event;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Day;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Food;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Water;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Sanity;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_AdvanceDay;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Button_AdvanceDay;

	UFUNCTION()
	void OnAdvanceDayClicked();
};
