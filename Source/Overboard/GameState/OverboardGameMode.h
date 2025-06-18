// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Overboard/Items/ItemStruct.h"
#include "Overboard/Items/Item.h"
#include "OverboardGameMode.generated.h"

UCLASS(minimalapi)
class AOverboardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOverboardGameMode();

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float TimeLimit = 120.f; // seconds before ship sinks

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float RemainingTime;

	UFUNCTION(BlueprintCallable)
	void EndPhase();
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UOverboardWidget> OverboardUIClass;

	UPROPERTY()
	UOverboardWidget* OverboardWidget;

	/** Called when an item is successfully deposited into the raft */
	void RegisterCollectedItem(AItem* Item);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, int32> DepositedItems;

protected:
	FTimerHandle CountdownTimerHandle;

	void TickCountdown();
};



