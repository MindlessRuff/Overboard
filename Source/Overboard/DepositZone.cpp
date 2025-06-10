// Fill out your copyright notice in the Description page of Project Settings.


#include "DepositZone.h"

#include "InterchangeResult.h"
#include "GameState/OverboardGameMode.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ADepositZone::ADepositZone()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	RootComponent = TriggerZone;
	TriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//TriggerZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	//TriggerZone->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	TriggerZone->SetGenerateOverlapEvents(true);
}


// Called when the game starts or when spawned
void ADepositZone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADepositZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADepositZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor)
	{
		// Your overlap logic here
		UE_LOG(LogTemp, Display, TEXT("Actor %s began overlapping with %s"), *GetName(), *OtherActor->GetName());
	}
}
