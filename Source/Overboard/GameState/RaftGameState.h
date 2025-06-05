// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RaftGameState.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API ARaftGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	
	ARaftGameState();

	UPROPERTY(BlueprintReadOnly, Category = "Survival")
	int32 CurrentDay;

	UPROPERTY(BlueprintReadOnly, Category = "Survival")
	int32 Food;

	UPROPERTY(BlueprintReadOnly, Category = "Survival")
	int32 Water;

	UPROPERTY(BlueprintReadOnly, Category = "Survival")
	int32 Sanity;

	UFUNCTION(BlueprintCallable, Category = "Survival")
	void AdvanceDay();

protected:
	virtual void BeginPlay() override;
	void ApplyDailyDecay();
	void TriggerRandomEvent();
	void CheckGameOver();
};
