// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameStateBase.h"
#include "RaftGameState.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FRaftDayTextEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Day;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text;
};

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
	
	UPROPERTY(BlueprintReadOnly, Category = "Survival")
	FText Text_Event;

	UPROPERTY(BlueprintReadOnly, Category = "Day Text")
	TArray<FRaftDayTextEntry> LoadedEntries;

	UFUNCTION(BlueprintCallable, Category = "Survival")
	void AdvanceDay();

protected:
	void StartIntro();
	virtual void BeginPlay() override;
	void ApplyDailyDecay();
	void TriggerRandomEvent();
	void CheckGameOver();
};
