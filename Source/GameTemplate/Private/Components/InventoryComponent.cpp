// (C) Anastasis Marinos 2025 //

#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* Item) const
{
	if(Item)
	{
		if(InventoryContents.Contains(Item))
		{
			return Item;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* Item) const
{
	if(Item)
	{
		if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(Item))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* Item) const
{
	// Lambda Model
	if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([&Item](const UItemBase* InventoryItem)
		{
			return InventoryItem->ID == Item->ID && !InventoryItem->IsFullItemStack();
		}
		))
	{
		return *Result;
	}
	
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* Item, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / Item->GetItemSingleWeight());
	if(WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->ItemNumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* Item, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, Item->Quantity);

	Item->SetQuantity(Item->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * Item->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* Item, const int32 AmountToSplit)
{
	if(!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(Item, AmountToSplit);
		AddNewItem(Item, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* Item)
{
	// Check if in the input item has valid weight.
	if(FMath::IsNearlyZero(Item->GetItemSingleWeight()) || Item->GetItemStackWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to inventory. Item has invalid weight value."), Item->ItemTextData.Name));
	}

	// Will the item weight overflow the weight capacity.
	if(InventoryTotalWeight + Item->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to inventory. Item would overflow weight limit."), Item->ItemTextData.Name));
	}

	if(InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to inventory. All inventory slots are full."), Item->ItemTextData.Name));
	}

	AddNewItem(Item, 1);
	return FItemAddResult::AddedAll(1,FText::Format(
		FText::FromString("Successfully added {0} {1} to inventory."), 1, Item->ItemTextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* Item, int32 RequestedAddAmount)
{
	if(RequestedAddAmount <= 0 || FMath::IsNearlyZero(Item->GetItemStackWeight()))
	{
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;

	// Check if the input item already exists and is not a full stack.
	UItemBase* ExistingItemStack = FindNextPartialStack(Item);

	// Distribute item stack over existing stacks.
	while(ExistingItemStack)
	{
		// Calculate how many of the existing item would be needed to make the next ful stack.
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		// Calculate how many of the AmountToMakeFullStack can actually be carried based on weight capacity.
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// As long as the remaining amount of the item does not overflow stack capacity.
		if(WeightLimitAddAmount > 0)
		{
			// Adjust the existing items stack quantity and inventory total weight.
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			// Adjust the count to be distributed.
			AmountToDistribute -= WeightLimitAddAmount;

			Item->SetQuantity(AmountToDistribute);
			
			// If max weight capacity would be exceeded by another item, return early.
			if(InventoryTotalWeight + ExistingItemStack->GetItemSingleWeight() > InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			if(AmountToDistribute != RequestedAddAmount)
			{
				// This block will be reached if distributing an item across multiple stacks,
				// and the weight limit is hit during that process.
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
			return 0;
		}
		
		if(AmountToDistribute <= 0)
		{
			// All of the input item was distributed across existing stacks.
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// Check if there is still another valid partial stack.
		ExistingItemStack = FindNextPartialStack(Item);
	}

	// No more partial stacks found, check if new stack can be added.
	if(InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		// Attempt to add as many from the remaining item quantity that can fit inventory weight capacity;
		const int32 WeightLimitAmount = CalculateWeightAddAmount(Item, AmountToDistribute);

		if(WeightLimitAmount > 0)
		{
			// If there is still more item to distribute, but weight limit has been reached.
			if(WeightLimitAmount < AmountToDistribute)
			{
				// Adjust the input item and add a new stack with as many as can be held.
				AmountToDistribute -= WeightLimitAmount;
				Item->SetQuantity(AmountToDistribute);

				// Create a copy since only a partial stack is being added.
				AddNewItem(Item->CreateItemCopy(), WeightLimitAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			// Otherwise, the full remainder of the stack can be added.
			AddNewItem(Item, AmountToDistribute);
			return RequestedAddAmount;
		}
		// Reached if there is free item slots, but no remaining weight capacity.
		OnInventoryUpdated.Broadcast();
		return RequestedAddAmount - AmountToDistribute;
	}
	// Can only be reached if there is no existing stack and no extra capacity slots.
	return 0;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if(GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// Handle non-stackable items.
		if(!InputItem->ItemNumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		// Handle stackable items.
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if(StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
				FText::FromString("Successfully added {0} {1} to the inventory."), InitialRequestedAddAmount, InputItem->ItemTextData.Name));
		}

		if(StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Partial amount of {0} added to the inventory. Number Added = {1}."),InputItem->ItemTextData.Name, StackableAmountAdded));
		}

		if(StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots or invalid item."),InputItem->ItemTextData.Name));
		}
	}
	check(false);
	return FItemAddResult::AddedNone(FText::FromString("Try add item fallthrough error. GetOwner() check failed."));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 Amount)
{
	UItemBase* NewItem;

	if(Item->bIsCopy || Item->bIsPickup)
	{
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(Amount);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}


