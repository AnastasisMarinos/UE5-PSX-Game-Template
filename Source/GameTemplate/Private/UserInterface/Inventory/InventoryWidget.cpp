// © Anastasis Marinos 2025 //

#include "UserInterface/Inventory/InventoryWidget.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

#include "Player/PlayerCharacter.h"
#include "Items/ItemBase.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if(InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryWidget::RefreshInventory);
			SetInfoText();
		}
	}
}

void UInventoryWidget::SetInfoText() const
{
	const FString WeightInfoValue{"Weight: " +
		FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/"
		+ FString::SanitizeFloat(InventoryReference->GetWeightCapacity())
	};

	const FString CapacityInfoValue{"Capacity: " +
		FString::FromInt(InventoryReference->GetInventoryContents().Num()) + "/"
		+ FString::FromInt(InventoryReference->GetSlotCapacity())
	};
	
	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
}

void UInventoryWidget::RefreshInventory()
{
	if(InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for(UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemRefrence(InventoryItem);

			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}
		SetInfoText();
	}
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	if (!ItemDragDrop || !ItemDragDrop->SourceItem || !InventoryReference)
		return false;

	UInventoryComponent* SourceInventory = ItemDragDrop->SourceInventory;
	if (!SourceInventory || SourceInventory == InventoryReference)
		return false;

	UItemBase* SourceItem = ItemDragDrop->SourceItem;
	FItemAddResult TransferResult = SourceInventory->TransferItemTo(InventoryReference, SourceItem, SourceItem->Quantity);

	UE_LOG(LogTemp, Warning, TEXT("Item transferred to player inventory: %s"), *TransferResult.ResultMessage.ToString());
	return TransferResult.OperationResult != EItemAddResult::IAR_NoItemAdded;
}
