// (C) Anastasis Marinos 2025 //

#include "Items/ItemBase.h"
#include "Items/ItemConsumable.h"

#include "Components/InventoryComponent.h"
#include "Player/PlayerCharacter.h"

// Constructor – initializes base item defaults.
UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{
	// Assign default color based on item quality.
	switch (ItemQuality) // TODO: Use this to affect hover material in post-process.
	{
	case EItemQuality::Casual:
		ItemQualityColor = FLinearColor(0.11f, 0.45f, 0.11f, 1.0f);
		break;
	case EItemQuality::Premium:
		ItemQualityColor = FLinearColor(0.65f, 0.22f, 0.065f, 1.0f);
		break;
	case EItemQuality::Luxurious:
		ItemQualityColor = FLinearColor(0.23f, 0.12f, 0.65f, 1.0f);
		break;
	}
}

// Resets flags for internal usage, e.g., when creating a copy or dropping.
void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

// Creates a deep copy of this item (used for inventory or drops)
UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy;
	switch (ItemType) // Extendable for different derived item types.
	{
	case EItemType::Consumable:
		ItemCopy = NewObject<UItemConsumable>(StaticClass());
		break;
	default: 
		ItemCopy = NewObject<UItemBase>(StaticClass());
	}
	
	// Copy all relevant data.
	ItemCopy->ID = ID;
	ItemCopy->Quantity = Quantity;
	ItemCopy->ItemQuality = ItemQuality;
	ItemCopy->ItemType = ItemType;
	ItemCopy->ItemTextData = ItemTextData;
	ItemCopy->ItemNumericData = ItemNumericData;
	ItemCopy->ItemStatistics = ItemStatistics;
	ItemCopy->ItemAssetData = ItemAssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

// Sets the quantity of this item, automatically removing from inventory if <= 0.
void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0,  ItemNumericData.bIsStackable ? ItemNumericData.MaxStackSize : 1);

		if (OwningInventory && Quantity <= 0)
		{
			OwningInventory->RemoveSingleInstanceOfItem(this);
		}
	}
}

// Splits a stackable item in half (used for player actions like splitting inventory stacks)
void UItemBase::SplitStack()
{
	if (OwningInventory && Quantity > 1)
	{
		ResetItemFlags();
		OwningInventory->SplitExistingStack(this, Quantity / 2);
	}
}

// Uses the item (plays sound and animation)
void UItemBase::UseItem(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->UseItem(ItemAssetData.UsageSound, ItemAssetData.UsageAnim);
}