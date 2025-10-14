// © Anastasis Marinos //

#include "World/Interactables/PickupBase.h"

#include "Components/InventoryComponent.h"
#include "Player/PlayerCharacter.h"
#include "Items/ItemBase.h"

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = false; // This actor doesn’t need to tick every frame (performance optimization)

	// Create the mesh component used to represent the pickup in the world
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("Pickup Mesh");
	SetRootComponent(PickupMesh); // Set mesh as root

	// Configure collision: ignore most channels, but block visibility, camera, and world static for trace hits
	PickupMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	PickupMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	PickupMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// Enable physics simulation for natural movement
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->SetLinearDamping(2.0f);
	PickupMesh->SetAngularDamping(2.0f);
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the pickup with a default item type and quantity
	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
}

void APickupBase::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 Quantity)
{
	// Load item data from data table if valid
	if(!ItemRowHandle.IsNull())
	{
		const FItemData* ItemData = ItemRowHandle.GetRow<FItemData>(ItemRowHandle.RowName.ToString());

		// Create a new item object based on the data row
		ItemReference = NewObject<UItemBase>(this, BaseClass);

		// Copy relevant data from data table row to runtime item
		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->ItemStatistics = ItemData->ItemStatistics;
		ItemReference->ItemNumericData = ItemData->ItemNumericData;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->ItemAssetData = ItemData->ItemAssetData;

		InteractionAnim = ItemReference->ItemAssetData.InteractionAnim;
		InteractionSound = ItemReference->ItemAssetData.InteractionSound;

		// Determine if item can stack
		ItemReference->ItemNumericData.bIsStackable = ItemData->ItemNumericData.MaxStackSize > 1;
		Quantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(Quantity);

		// Update the pickup’s mesh to match the item
		PickupMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);

		// Update the interactable display data
		UpdateInteractableData();
	}
}

void APickupBase::InitializeDrop(UItemBase* ItemToDrop, const int32 Quantity)
{
	// Initialize pickup from an existing item instance (used when dropping from inventory)
	ItemReference = ItemToDrop;

	Quantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(Quantity);
	ItemReference->ItemNumericData.Weight = ItemToDrop->GetItemSingleWeight();
	ItemReference->OwningInventory = nullptr; // No longer belongs to an inventory

	PickupMesh->SetStaticMesh(ItemToDrop->ItemAssetData.Mesh);

	UpdateInteractableData();
}

void APickupBase::UpdateInteractableData()
{
	// Sets the data that will appear in the interaction widget (UI prompt)
	InstanceInteractableData.InteractableType = EInteractableType::Item;
	InstanceInteractableData.Action = ItemReference->ItemTextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->ItemTextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void APickupBase::BeginFocus()
{
	// Enable outline/highlight when player looks at the pickup
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickupBase::EndFocus()
{
	// Disable highlight when player stops looking at the pickup
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickupBase::Interact(APlayerCharacter* PlayerCharacter)
{
	// Called when the player interacts (presses use)
	if(PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
		Super::Interact(PlayerCharacter);
	}
}

void APickupBase::TakePickup(APlayerCharacter* Taker)
{
	// Adds the pickup’s item to the player’s inventory and handles stack/overflow logic
	if(!IsPendingKillPending())
	{
		if(ItemReference)
		{
			if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch (AddResult.OperationResult)
				{
				case IAR_NoItemAdded: // Inventory full or invalid
					break;
				case IAR_PartialAmountItemAdded: // Only part of stack added
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				case IAR_AllItemAdded: // Entire stack successfully added
					Destroy();
					break;
				}

				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player inventory component is null!"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pickup internal item reference was null!"));
		}
	}
}

#if WITH_EDITOR
// Editor-only: automatically updates mesh in editor when you change item row
void APickupBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName))
	{
		if (!ItemRowHandle.IsNull())
		{
			const FItemData* ItemData = ItemRowHandle.GetRow<FItemData>(ItemRowHandle.RowName.ToString());
			PickupMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);
		}
	}
}
#endif