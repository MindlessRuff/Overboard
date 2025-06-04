// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"


// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(ItemMesh);
	CollisionSphere->InitSphereRadius(100.f);

	ItemName = TEXT("Item");
	bIsPickedUp = false;
	RunningTime = 0.f;

	RotationAxis.Normalize();
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldAnimate && !bIsPickedUp)
	{
		AnimateItem(DeltaTime);
	}
}

void AItem::AnimateItem(float DeltaTime)
{
	RunningTime += DeltaTime;

	FVector NewLocation = InitialLocation;
	float HeightOffset = (FMath::Sin(RunningTime * RotationSpeed) * 0.5f + 0.5f) * FloatingHeight;
	NewLocation.Z += HeightOffset;

	FRotator CurrentRotation = GetActorRotation();
	FRotator DeltaRotation = FRotator::ZeroRotator;

	if (RotationAxis.X != 0.f) DeltaRotation.Roll += DeltaTime * RotationSpeed;
	if (RotationAxis.Y != 0.f) DeltaRotation.Pitch += DeltaTime * RotationSpeed;
	if (RotationAxis.Z != 0.f) DeltaRotation.Yaw += DeltaTime * RotationSpeed;

	FRotator NewRotation = CurrentRotation + DeltaRotation;
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void AItem::PickUp(AActor* PickUpActor)
{
	if (!bIsPickedUp)
	{
		bIsPickedUp = true;
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

