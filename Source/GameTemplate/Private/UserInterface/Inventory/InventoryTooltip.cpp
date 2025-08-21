// (C) Anastasis Marinos 2025 //

#include "UserInterface/Inventory/InventoryTooltip.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"

#include "Components/TextBlock.h"
#include "Items/ItemBase.h"


void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventoryItemSlotHovered->GetItemRefrence();

	switch (ItemBeingHovered->ItemType)
	{
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		ItemType->SetColorAndOpacity(FLinearColor::Blue);
		break;
	case EItemType::Weapon:
		break;
	case EItemType::Key:
		break;
	default: ;
	}
	ItemName->SetText(ItemBeingHovered->ItemTextData.Name);
	ItemDescription->SetText(ItemBeingHovered->ItemTextData.Description);

	const FString WeightInfo = {"Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight()) + "kg"};
	StackWeight->SetText(FText::FromString(WeightInfo));

	if(ItemBeingHovered->ItemNumericData.bIsStackable)
	{
		const FString StackSizeInfo = {"Items: " + FString::FromInt(ItemBeingHovered->ItemNumericData.MaxStackSize)};
		MaxStackSize->SetText(FText::FromString(StackSizeInfo));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
