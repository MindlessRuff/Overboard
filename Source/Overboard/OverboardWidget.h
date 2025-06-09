// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverboardWidget.generated.h"


class UTextBlock;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class OVERBOARD_API UOverboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateTimeDisplay(float TimeRemaining);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeRemainingText;

};
