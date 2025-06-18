// Fill out your copyright notice in the Description page of Project Settings.


#include "DepositZone.h"

#include "InterchangeResult.h"
#include "Character/OverboardCharacter.h"
#include "GameState/OverboardGameMode.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ADepositZone::ADepositZone()
{
    
	PrimaryActorTick.bCanEverTick = false;
}


// Called when the game starts or when spawned
void ADepositZone::BeginPlay()
{
	Super::BeginPlay();
	// First, try to find the component if it wasn't created in constructor
	if (!TriggerZone)
	{
		TriggerZone = FindComponentByClass<UBoxComponent>();
		UE_LOG(LogTemp, Warning, TEXT("TriggerZone was null, tried to find existing component"));
	}
    
	if (TriggerZone)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting up TriggerZone collision"));
        
		TriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		TriggerZone->SetCollisionResponseToAllChannels(ECR_Ignore);
		TriggerZone->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		TriggerZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		TriggerZone->SetGenerateOverlapEvents(true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TriggerZone is still null! Check Blueprint setup"));
	}
}

// Called every frame
void ADepositZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADepositZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	AItem* Item = Cast<AItem>(OtherActor);
	if (!Item)
	{
		return; // Not an item, ignore
	}
	
	// Log item data
	UE_LOG(LogTemp, Display, TEXT("Item entered deposit zone - Name: %s, Type: %s, Quantity: %d"), 
		   *Item->GetItemName().ToString(), 
		   *Item->GetItemType(), 
		   Item->GetQuantity());

	Item->SetDepositable(true);

}

void ADepositZone::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	AItem* Item = Cast<AItem>(OtherActor);
	if (!Item)
	{
		return; // Not an item, ignore
	}
	
	// Log item data
	UE_LOG(LogTemp, Display, TEXT("Item exited deposit zone - Name: %s, Type: %s, Quantity: %d"), 
		   *Item->GetItemName().ToString(), 
		   *Item->GetItemType(), 
		   Item->GetQuantity());

	Item->SetDepositable(false);

}