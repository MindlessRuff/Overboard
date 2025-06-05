// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
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
    
	// Show or hide interaction prompt
	UFUNCTION(BlueprintCallable, Category = "Item")
	void ShowInteractionPrompt(bool bShow);
	
	// Toggle item highlighting
	UFUNCTION(BlueprintCallable, Category = "Item")
	
	void SetHighlighted(bool bHighlight);
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsHighlighted() const { return bIsHighlighted; }

	// Getters for item state
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsPickupable() const { return !bIsPickedUp; }
	
	// Get held properties for the player to use
	UFUNCTION(BlueprintCallable, Category = "Held State")
	float GetHeldDistance() const { return HeldDistance; }
    
	UFUNCTION(BlueprintCallable, Category = "Held State")
	FVector GetHeldOffset() const { return HeldOffset; }
    
	UFUNCTION(BlueprintCallable, Category = "Held State")
	FRotator GetHeldRotation() const { return HeldRotation; }
    
	UFUNCTION(BlueprintCallable, Category = "Held State")
	float GetHeldScale() const { return HeldScale; }


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CollisionSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* InteractionPrompt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemDescription;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString InteractionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float InteractionRange = 300.0f;
	
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
	float AnimationSmoothing = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (EditCondition = "bShouldAnimate"))
	FVector RotationAxis = FVector(0.f, 0.f, 1.f);

	// Held Item settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Held State")
	float HeldDistance = 80.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Held State")
	FVector HeldOffset = FVector(20.0f, 0.0f, -10.0f);
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Held State")
	float HeldScale = 0.7f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Held State")
	FRotator HeldRotation = FRotator(0.0f, 0.0f, 0.0f);

	// Highlight settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Highlight")
	UMaterialInterface* DefaultMaterial;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Highlight")
	UMaterialInterface* HighlightMaterial;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Highlight")
	float HighlightIntensity = 1.5f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Highlight")
	FLinearColor HighlightColor = FLinearColor(1.0f, 0.8f, 0.0f, 1.0f); // Gold color



private:
	float RunningTime;
	FVector InitialLocation;
    FVector InitialScale;
	
    bool bIsHighlighted;

	AActor* HeldBy;
    
	// Previous velocity for calculating sway
	FVector PreviousVelocity = FVector::ZeroVector;


	// Material instances for the item
	UPROPERTY()
	UMaterialInstanceDynamic* DefaultMaterialInstance;
    
	UPROPERTY()
	UMaterialInstanceDynamic* HighlightMaterialInstance;


	void AnimateItem(float DeltaTime);
	
	void SetupMaterials();

};
