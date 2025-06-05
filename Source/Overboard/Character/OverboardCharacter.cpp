// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverboardCharacter.h"

#include "EngineUtils.h"
#include "OverboardProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "IDetailTreeNode.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AOverboardCharacter

AOverboardCharacter::AOverboardCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	HeldItemAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("HeldItemAnchor"));
	HeldItemAnchor->SetupAttachment(FirstPersonCameraComponent);

	// Offset: Forward 100 units, down 50 units (tweak to taste)
	HeldItemAnchor->SetRelativeLocation(FVector(100.f, 0.f, -50.f));
	HeldItemAnchor->SetRelativeRotation(FRotator::ZeroRotator);  // Optional

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	HeldItem = nullptr;
	CurrentInteractable = nullptr;
	
	// Add held item variables for extra effects
	HeldItemBobSpeed = 2.0f;
	HeldItemBobAmount = 3.0f;
}
void AOverboardCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("CameraComponent = %s | Location = %s | Rotation = %s"),
	*FirstPersonCameraComponent->GetName(),
	*FirstPersonCameraComponent->GetComponentLocation().ToString(),
	*FirstPersonCameraComponent->GetComponentRotation().ToString());

	
}
void AOverboardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();
}

void AOverboardCharacter::CheckForInteractables()
{
	// If we're already holding an item, don't look for new interactables
	if (HeldItem != nullptr)
	{
		// Hide prompt on any previously detected interactable
		if (CurrentInteractable != nullptr)
		{
			CurrentInteractable->ShowInteractionPrompt(false);
			CurrentInteractable->SetHighlighted(false);
			CurrentInteractable = nullptr;
		}
		return;
	}
    
	// Perform trace to find interactable objects
	FHitResult HitResult = PerformInteractionTrace();
    
	// Check if we hit an interactable object
	AItem* HitItem = Cast<AItem>(HitResult.GetActor());
    
	// If we're looking at a new interactable
	if (HitItem && HitItem->IsPickupable())
	{
		// If this is a different interactable than before
		if (CurrentInteractable != HitItem)
		{
			// Remove highlight and hide prompt on previous interactable
			if (CurrentInteractable != nullptr)
			{
				CurrentInteractable->ShowInteractionPrompt(false);
				CurrentInteractable->SetHighlighted(false);
			}
            
			// Show prompt and highlight new interactable
			HitItem->ShowInteractionPrompt(true);
			HitItem->SetHighlighted(true);
			CurrentInteractable = HitItem;
		}
	}
}

FHitResult AOverboardCharacter::PerformInteractionTrace()
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);
    
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * InteractionDistance);
    
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
    
	// Optional: visualize the trace for debugging
	// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);
    
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByProfile(HitResult, TraceStart, TraceEnd, TEXT("BlockAll"), QueryParams);
    
	return HitResult;
}

void AOverboardCharacter::DropHeldItem()
{
}

//////////////////////////////////////////////////////////////////////////// Input

void AOverboardCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AOverboardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOverboardCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOverboardCharacter::Look);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AOverboardCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"
										   "This template is built to use the Enhanced Input system."
			 "If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}



void AOverboardCharacter::PickUpItem(AItem* Item)
{
	
	UE_LOG(LogTemp, Display, TEXT("Try to attach item to camera"));
	if (Item && !HeldItem)
	{
		HeldItem = Item;
		
		Item->PickUp(this);
		Item->AttachToComponent(HeldItemAnchor, FAttachmentTransformRules::SnapToTargetIncludingScale);
		UE_LOG(LogTemp, Display, TEXT("Item Attached To: %s"), *Item->GetAttachParentActor()->GetName());
		
		Item->SetActorRelativeLocation(FVector(75.f, 0.f, 30.f)); // Offset in front of view
		Item->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));

		UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Item->GetRootComponent());
		if (Prim)
		{
			Prim->SetSimulatePhysics(false);
			Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		

		UE_LOG(LogTemp, Display, TEXT("Picked up %s"), *Item->GetItemName());
		
	}
}

void AOverboardCharacter::DepositItem()
{
	FHitResult HitResult;
	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndLocation = StartLocation + FirstPersonCameraComponent->GetForwardVector() * 2000.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		if (HitResult.GetActor()->ActorHasTag("DepositZone"))
		{
			if (HeldItem)
			{
				UE_LOG(LogTemp, Display, TEXT("Deposited %s"), *HeldItem->GetItemName());
				HeldItem = nullptr;
			}
		}
	}
}

void AOverboardCharacter::Interact()
{
	if (HeldItem != nullptr)
	{
		DepositItem();
		return;
	}

	// If we have a valid interactable
	if (CurrentInteractable && CurrentInteractable->IsPickupable())
	{
		PickUpItem(CurrentInteractable);
        
		// Store reference to held item
		HeldItem = CurrentInteractable;
		CurrentInteractable = nullptr;

		HeldItemTime = 0.0f;
		PreviousCameraRotation = FirstPersonCameraComponent->GetComponentRotation();
	}
}

void AOverboardCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AOverboardCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}