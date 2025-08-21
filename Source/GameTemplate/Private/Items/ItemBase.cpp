// (C) Anastasis Marinos 2025 //

#include "Items/ItemBase.h"
#include "Items/ItemConsumable.h"

#include "Components/InventoryComponent.h"
#include "Player/PlayerCharacter.h"


UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{
	switch (ItemQuality) // TODO Affect item hover material in post process.
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

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy;
	switch (ItemType) // TODO Add other item types when implemented.
	{
	case EItemType::Consumable:
		ItemCopy = NewObject<UItemConsumable>(StaticClass());
	break;
	default: ItemCopy = NewObject<UItemBase>(StaticClass());
	}
	
	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemTextData = this->ItemTextData;
	ItemCopy->ItemNumericData = this->ItemNumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->ItemAssetData = this->ItemAssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0,  ItemNumericData.bIsStackable ? ItemNumericData.MaxStackSize : 1);

		if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
	}
}

void UItemBase::SplitStack()
{
	if (OwningInventory)
	{
		if(Quantity > 1)
		{
			ResetItemFlags();
			OwningInventory->SplitExistingStack(this, Quantity/2);
		}
	}
}

void UItemBase::UseItem(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->UseItem(ItemAssetData.UsageSound, ItemAssetData.UsageAnim);
}


