// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class UButton;
class USizeBox;
class UInventoryItemInteract;
class UTextBlock;
class UImage;
class UBorder;
class UInventoryTooltip;
class UInventoryDragItem;
class UItemBase;


UCLASS()
class GAMETEMPLATE_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	// FUNCTIONS //
	FORCEINLINE void SetItemRefrence(UItemBase* InputItem) { ItemRefrence = InputItem; }
	FORCEINLINE UItemBase* GetItemRefrence() const { return ItemRefrence; }

protected:
	// PROPERTIES & VARIABLES //
	UPROPERTY(VisibleAnywhere ,Category="Inventory Slot")
	UItemBase* ItemRefrence;
	
	UPROPERTY(VisibleAnywhere ,Category="Inventory Slot", meta=(BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere ,Category="Inventory Slot", meta=(BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere ,Category="Inventory Slot", meta=(BindWidget))
	UTextBlock* ItemQuantity;

	UPROPERTY(VisibleAnywhere ,Category="Inventory Slot", meta=(BindWidget))
	USizeBox* ItemInteractionBox;

	UPROPERTY(VisibleAnywhere ,Category="Inventory Slot", meta=(BindWidget))
	UButton* ItemUsageButton;
	
	UPROPERTY(VisibleAnywhere ,Category="Inventory Slot", meta=(BindWidget))
	UTextBlock* ItemUsageText;

	UPROPERTY(VisibleAnywhere ,Category="Inventory Slot", meta=(BindWidget))
	UButton* ItemDropButton;

	UPROPERTY(VisibleAnywhere ,Category="Inventory Slot", meta=(BindWidget))
	UButton* ItemSplitButton;

	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UInventoryDragItem> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UInventoryTooltip> ToolTipClass;

	UInventoryTooltip* Tooltip;

	// FUNCTIONS //
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	
	UFUNCTION()
	void UseItem();

	UFUNCTION()
	void SplitItem();

	UFUNCTION()
	void DropItem();

private:
	// PROPERTIES & VARIABLES //
	bool bIsInteractingWithItem = false;
};
