// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaftGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API ARaftGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class URaftSurvivalWidget> RaftUIClass;

public:
	ARaftGameMode();
	virtual void BeginPlay() override;
	
};
