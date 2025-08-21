// (C) Anastasis Marinos 2025 //

#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "UserInterface/Inventory/InventoryDragItem.h"
#include "UserInterface/Inventory/InventoryTooltip.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

#include "Items/ItemBase.h"
#include "Player/PlayerCharacter.h"


void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(ToolTipClass)
	{
		Tooltip = CreateWidget<UInventoryTooltip>(this, ToolTipClass);
		Tooltip->InventoryItemSlotHovered = this;
		SetToolTip(Tooltip);
	}
	ItemInteractionBox->SetVisibility(ESlateVisibility::Collapsed);

	SetIsFocusable(true);
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if(ItemRefrence)
	{
		ItemBorder->SetBrushColor(ItemRefrence->ItemQualityColor);
		ItemIcon->SetBrushFromTexture(ItemRefrence->ItemAssetData.Icon);

		if(ItemRefrence->ItemNumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemRefrence->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}

		ItemUsageText->SetText(ItemRefrence->ItemTextData.UsageText);
	}

	ItemUsageButton->OnClicked.AddDynamic(this, &UInventoryItemSlot::UseItem);
	ItemSplitButton->OnClicked.AddDynamic(this, &UInventoryItemSlot::SplitItem);
	ItemDropButton->OnClicked.AddDynamic(this, &UInventoryItemSlot::DropItem);
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		SetToolTip(nullptr);
		if(bIsInteractingWithItem)
		{
			ItemInteractionBox->SetVisibility(ESlateVisibility::Collapsed);
			bIsInteractingWithItem = false;
		}
		else
		{
			ItemInteractionBox->SetVisibility(ESlateVisibility::Visible);
			bIsInteractingWithItem = true;
		}
		return Reply.Handled();
	}
	
	return Reply.Unhandled();
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	SetToolTip(Tooltip);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if(DragItemVisualClass)
	{
		const TObjectPtr<UInventoryDragItem> DragVisual = CreateWidget<UInventoryDragItem>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemRefrence->ItemAssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		ItemRefrence->ItemNumericData.bIsStackable
			? DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemRefrence->Quantity))
			: DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		
		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemRefrence;
		DragItemOperation->SourceInventory = ItemRefrence->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::CenterCenter;

		OutOperation = DragItemOperation;
	}
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryItemSlot::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	ItemInteractionBox->SetVisibility(ESlateVisibility::Collapsed);
	bIsInteractingWithItem = false;
}

void UInventoryItemSlot::UseItem()
{
	ItemRefrence->UseItem(Cast<APlayerCharacter>(GetOwningPlayerPawn()));
}

void UInventoryItemSlot::SplitItem()
{
	ItemRefrence->SplitStack();
}

void UInventoryItemSlot::DropItem()
{
	Cast<APlayerCharacter>(GetOwningPlayerPawn())->DropItem(ItemRefrence, ItemRefrence->Quantity);
}
