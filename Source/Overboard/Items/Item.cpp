// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "ContentBrowserDataSource.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"


// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
	ItemMesh->SetCollisionProfileName(TEXT("BlockAll"));
	ItemMesh->SetSimulatePhysics(true);

	// Create widget component for floating text
	InteractionPrompt = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPrompt"));
	InteractionPrompt->SetupAttachment(RootComponent);
	InteractionPrompt->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionPrompt->SetDrawSize(FVector2D(200.0f, 50.0f));
	InteractionPrompt->SetVisibility(false);
	InteractionPrompt->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	
	ItemName = TEXT("Item");
	ItemDescription = TEXT("This is an item.");
	InteractionText = "Press E to pick up!";
	bIsPickedUp = false;
	RunningTime = 0.f;
	HeldBy = nullptr;
	
	RotationAxis.Normalize();
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	InitialScale = GetActorScale3D();
	
	if (InteractionPrompt)
	{
		// Set draw size if not already set
		InteractionPrompt->SetDrawSize(FVector2D(200.0f, 50.0f));
        
		// Make sure the widget class is set
		if (!InteractionPrompt->GetWidgetClass())
		{
			UE_LOG(LogTemp, Warning, TEXT("Interaction Prompt Widget Class is not set!"));
		}
        
		// Create the widget instance if needed
		if (!InteractionPrompt->GetUserWidgetObject())
		{
			InteractionPrompt->InitWidget();
		}
        
		// Set widget properties
		InteractionPrompt->SetWidgetSpace(EWidgetSpace::Screen);
		InteractionPrompt->SetVisibility(false); // Start hidden
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InteractionPrompt component is null!"));
	}

	SetupMaterials();

}

void AItem::SetupMaterials()
{
	// If a highlight material is provided, use it
	if (HighlightMaterial)
	{
		HighlightMaterialInstance = UMaterialInstanceDynamic::Create(HighlightMaterial, this);
		HighlightMaterialInstance->SetVectorParameterValue("HighlightColor", HighlightColor);
		HighlightMaterialInstance->SetScalarParameterValue("HighlightIntensity", HighlightIntensity);
	}
	else
	{
		// If no highlight material provided, create a dynamic instance of the current material
		// and we'll modify it for highlighting
		UMaterialInterface* CurrentMaterial = ItemMesh->GetMaterial(0);
		if (CurrentMaterial)
		{
			DefaultMaterial = CurrentMaterial;
			DefaultMaterialInstance = UMaterialInstanceDynamic::Create(CurrentMaterial, this);
			ItemMesh->SetMaterial(0, DefaultMaterialInstance);
            
			// Create a copy for the highlight state
			HighlightMaterialInstance = UMaterialInstanceDynamic::Create(CurrentMaterial, this);
			HighlightMaterialInstance->SetScalarParameterValue("Emissive", 1.0f);
			HighlightMaterialInstance->SetVectorParameterValue("EmissiveColor", HighlightColor);
		}
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Check if player is nearby to show prompt
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (bIsPickedUp)
	{
		if (IsAttachedTo(HeldBy))
		{
			UE_LOG(LogTemp, Verbose, TEXT("Still attached to: %s"), *GetAttachParentActor()->GetName());
		}
	}
	else if (PlayerController != nullptr)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
            
			// Show prompt if player is within interaction range
			if (DistanceToPlayer <= InteractionRange && !bIsPickedUp)
			{
				ShowInteractionPrompt(true);
			}
			else
			{
				ShowInteractionPrompt(false);
			}
		}
	}

	if (bIsPickedUp && HeldBy)
	{
		
	}
	else if (bShouldAnimate && !bIsPickedUp)
	{
		if (ItemMesh->IsSimulatingPhysics())
		{
			ItemMesh->SetSimulatePhysics(false);
		}
		AnimateItem(DeltaTime);
	}
}

void AItem::AnimateItem(float DeltaTime)
{
	float ClampedDeltaTime = FMath::Min(DeltaTime, 0.033f);
	RunningTime += ClampedDeltaTime;

	float SmoothSine = FMath::Sin(RunningTime * RotationSpeed);
	float HeightOffset = (SmoothSine * 0.5f + 0.5f) * FloatingHeight;
	
	FVector NewLocation = InitialLocation;
	NewLocation.Z += HeightOffset;

	FRotator CurrentRotation = GetActorRotation();
	FRotator DeltaRotation = FRotator::ZeroRotator;

	if (RotationAxis.X != 0.f) DeltaRotation.Roll += ClampedDeltaTime * RotationSpeed;
	if (RotationAxis.Y != 0.f) DeltaRotation.Pitch += ClampedDeltaTime * RotationSpeed;
	if (RotationAxis.Z != 0.f) DeltaRotation.Yaw += ClampedDeltaTime * RotationSpeed;

	FRotator NewRotation = CurrentRotation + DeltaRotation;
	FVector CurrentLocation = GetActorLocation();
	FVector InterpolatedLocation = FMath::VInterpTo(CurrentLocation, NewLocation, ClampedDeltaTime, 5.0f);
	
	SetActorLocationAndRotation(InterpolatedLocation, NewRotation);
}

void AItem::ShowInteractionPrompt(bool bShow)
{
	if (InteractionPrompt != nullptr)
	{
		InteractionPrompt->SetVisibility(bShow);
		// If we have a valid widget instance, update its text
		if (bShow && InteractionPrompt->GetUserWidgetObject())
		{
			// Get the user widget
			UUserWidget* Widget = InteractionPrompt->GetUserWidgetObject();
            
			// Try to find and update the text component
			// The exact implementation depends on your widget setup
			// Example with a text block named "PromptText":
			UTextBlock* TextBlock = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("PromptText")));
			if (TextBlock)
			{
				TextBlock->SetText(FText::FromString(InteractionText));
			}
		}
	}
}

void AItem::SetHighlighted(bool bHighlight)
{
	// Only do something if the state is changing
	if (bIsHighlighted != bHighlight)
	{
		bIsHighlighted = bHighlight;
        
		// Apply the appropriate material
		if (bIsHighlighted && HighlightMaterialInstance)
		{
			ItemMesh->SetMaterial(0, HighlightMaterialInstance);
		}
		else if (DefaultMaterialInstance)
		{
			ItemMesh->SetMaterial(0, DefaultMaterialInstance);
		}
	}
}

void AItem::PickUp(AActor* PickUpActor)
{
	if (!bIsPickedUp)
	{
		bIsPickedUp = true;
		bShouldAnimate = false;
		HeldBy = PickUpActor;

		InteractionPrompt->SetVisibility(false);
		SetHighlighted(false);
		
	}
}

