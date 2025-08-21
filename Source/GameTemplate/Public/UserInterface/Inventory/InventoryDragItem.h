// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryDragItem.generated.h"

class UTextBlock;
class UImage;
class UBorder;


UCLASS()
class GAMETEMPLATE_API UInventoryDragItem : public UUserWidget
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES //
	UPROPERTY(VisibleAnywhere ,Category="DragItemVisual", meta=(BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere ,Category="DragItemVisual", meta=(BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere ,Category="DragItemVisual", meta=(BindWidget))
	UTextBlock* ItemQuantity;
};
