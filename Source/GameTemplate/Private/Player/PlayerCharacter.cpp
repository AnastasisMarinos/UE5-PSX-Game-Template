// © Anastasis Marinos 2025 //

#include "Player/PlayerCharacter.h"
#include "Player/PlayerHUD.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "World/Interactables/PickupBase.h"

// Constructor – sets up all default components and initial parameters for the player.
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true; // Enables per-frame updates (Tick).

	// --- Component Initialization --- //

	// Handles player interaction with world objects.
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));

	// Player’s personal inventory setup.
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Player Inventory"));
	InventoryComponent->SetSlotCapacity(20);
	InventoryComponent->SetWeightCapacity(50.f);
	
	// --- Camera & View Setup --- //
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraArm->SetupAttachment(GetMesh());
	CameraArm->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "head_socket");
	CameraArm->SetRelativeLocation(FVector(0,-5,24));
	CameraArm->TargetArmLength = 0.0f;
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->CameraRotationLagSpeed = 20.0f;

	// Player first-person camera setup.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	Camera->AttachToComponent(CameraArm, FAttachmentTransformRules::KeepRelativeTransform);

	// --- Mesh & Capsule Defaults --- //
	GetCapsuleComponent()->SetCapsuleRadius(26.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f,0.0f,-88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f,-90.0f,0));

	// --- Movement Defaults --- //
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->GravityScale = 1;
	GetCharacterMovement()->MaxAcceleration = 768;
	GetCharacterMovement()->BrakingFrictionFactor = 1.2;
	GetCharacterMovement()->GroundFriction = 7.5;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	GetCharacterMovement()->BrakingDecelerationWalking = 256;
	GetCharacterMovement()->JumpZVelocity = 450;
	GetCharacterMovement()->AirControl = .4;
	GetCharacterMovement()->MaxFlySpeed = 150;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200;
}

// Called when the game starts or the player spawns in the world.
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Cache a reference to the HUD for inventory and interaction.
	HUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	InteractionComponent->HUD = HUD;
}

// Updates the on-screen interaction widget (e.g., when looking at items).
void APlayerCharacter::UpdateInteractionWidget() const
{
	InteractionComponent->UpdateInteractionWidget();
}

// Drops an item into the world from the player’s inventory.
void APlayerCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if(InventoryComponent->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// Spawn the pickup a bit in front of the player.
		const FVector SpawnLocation{GetActorLocation() + (GetActorForwardVector() * 50.0f)};
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		// Remove from inventory and spawn world pickup.
		const int32 RemovedQuantity = InventoryComponent->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);
		APickupBase* Pickup = GetWorld()->SpawnActor<APickupBase>(APickupBase::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was null"));
	}
}

// Removes item from inventory when storing (e.g., into a chest or container).
void APlayerCharacter::StoreItem(UItemBase* ItemToStore, const int32 QuantityToStore)
{
	if(InventoryComponent->FindMatchingItem(ItemToStore))
	{
		InventoryComponent->RemoveAmountOfItem(ItemToStore, QuantityToStore);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was null"));
	}
}

// Plays sound and animation when interacting with an object.
void APlayerCharacter::InteractWithItem(USoundBase* InputUseSound, UAnimMontage* InputUseAnim)
{
	UGameplayStatics::PlaySound2D(GetWorld(), InputUseSound);
	PlayAnimMontage(InputUseAnim);
}

// Plays sound and animation when using an inventory item.
void APlayerCharacter::UseItem(USoundBase* InputUseSound, UAnimMontage* InputUseAnim)
{
	UGameplayStatics::PlaySound2D(GetWorld(), InputUseSound);
	PlayAnimMontage(InputUseAnim);
}