// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryTooltip.generated.h"

class UTextBlock;
class UInventoryItemSlot;


UCLASS()
class GAMETEMPLATE_API UInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES //
	UPROPERTY(VisibleAnywhere)
	UInventoryItemSlot* InventoryItemSlotHovered;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemName;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemType;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemDescription;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxStackSize;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* StackWeight;

protected:
	// FUNCTIONS //
	virtual void NativeConstruct() override;
};
