// (C) Anastasis Marinos 2025 //

#include "Player/PlayerCharacter.h"
#include "Player/PlayerHUD.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "World/Pickup.h"


// Initialize Character & Set Default Values.
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create & Initialize Character Components.
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Player Inventory"));
	InventoryComponent->SetSlotCapacity(20);
	InventoryComponent->SetWeightCapacity(50.f);
	
	// Initialize Camera Arm & Set Default Values.
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraArm->SetupAttachment(GetMesh());
	CameraArm->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "head_socket");
	CameraArm->SetRelativeLocation(FVector(0,-5,24));
	
	CameraArm->TargetArmLength = 0.0f;
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->CameraRotationLagSpeed = 20.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	Camera->AttachToComponent(CameraArm, FAttachmentTransformRules::KeepRelativeTransform);

	// Set Default Values For Character Capsule Component & Character Mesh.
	GetCapsuleComponent()->SetCapsuleRadius(26.0f);
	
	GetMesh()->SetRelativeLocation(FVector(0.0f,0.0f,-88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f,-90.0f,0));

	// Set Default Values For Character Movement.
	GetCharacterMovement()->GravityScale = 1;
	GetCharacterMovement()->MaxAcceleration = 768;
	GetCharacterMovement()->BrakingFrictionFactor = 1.2;
	GetCharacterMovement()-> GroundFriction = 7.5;
	GetCharacterMovement()->MaxWalkSpeed = 300;
	GetCharacterMovement()->BrakingDecelerationWalking = 256;
	GetCharacterMovement()->JumpZVelocity = 450;
	GetCharacterMovement()->AirControl = .4;
	GetCharacterMovement()->MaxFlySpeed = 150;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200;
}

// Called When The Game Starts Or When Spawned.
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	InteractionComponent->HUD = HUD;
}

void APlayerCharacter::UpdateInteractionWidget() const
{
	InteractionComponent->UpdateInteractionWidget();
}

void APlayerCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if(InventoryComponent->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{GetActorLocation() + (GetActorForwardVector() * 50.0f)};
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = InventoryComponent->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was null"));
	}
}

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

void APlayerCharacter::InteractWithItem(USoundBase* InputUseSound, UAnimMontage* InputUseAnim)
{
	UGameplayStatics::PlaySound2D(GetWorld(), InputUseSound);
	PlayAnimMontage(InputUseAnim);
}

void APlayerCharacter::UseItem(USoundBase* InputUseSound, UAnimMontage* InputUseAnim)
{
	UGameplayStatics::PlaySound2D(GetWorld(), InputUseSound);
	PlayAnimMontage(InputUseAnim);
}

