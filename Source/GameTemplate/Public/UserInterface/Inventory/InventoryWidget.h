// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class APlayerCharacter;
class UInventoryItemSlot;
class UInventoryComponent;
class UTextBlock;
class UWrapBox;


UCLASS()
class GAMETEMPLATE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES //
	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryPanel;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WeightInfo;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CapacityInfo;
	
	APlayerCharacter* PlayerCharacter;
	UInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;

	// FUNCTIONS //
	UFUNCTION()
	void RefreshInventory();
	
protected:
	// FUNCTIONS //
	void SetInfoText() const;

	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
