// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UInventoryComponent;
class UItemBase;


UCLASS()
class GAMETEMPLATE_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES //
	UPROPERTY()
	UItemBase* SourceItem;

	UPROPERTY()
	UInventoryComponent* SourceInventory;
};
