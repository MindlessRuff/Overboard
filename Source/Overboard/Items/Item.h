// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class OVERBOARD_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to be called when this actor is picked up
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void PickUp(AActor* PickUpActor);

	UFUNCTION(BlueprintCallable, Category = "Item")
	FString GetItemName() const { return ItemName; }
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CollisionSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bIsPickedUp;

	UPROPERTY(EditAnywhere, Category = "Animation")
	bool bShouldAnimate = true;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = (EditCondition = "bShouldAnimate"))
	float FloatingHeight = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (EditCondition = "bShouldAnimate"))
	float FloatingSpeed = .5f;
	
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (EditCondition = "bShouldAnimate"))
	float RotationSpeed = 15.f;
	
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (EditCondition = "bShouldAnimate"))
	FVector RotationAxis = FVector(0.f, 0.f, 1.f);

private:
	float RunningTime;
	FVector InitialLocation;

	void AnimateItem(float DeltaTime);

};
