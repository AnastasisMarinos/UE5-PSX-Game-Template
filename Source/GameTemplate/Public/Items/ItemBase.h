// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "Items/Data/ItemDataStructs.h"
#include "ItemBase.generated.h"

class APlayerCharacter;
class UInventoryComponent;


UCLASS()
class GAMETEMPLATE_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES //
	UPROPERTY()
	UInventoryComponent* OwningInventory;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Data", meta = (UIMin=1, UIMax=64))
	int32 Quantity;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	FName ID;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	EItemType ItemType;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	FItemTextData ItemTextData;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	FItemNumericData ItemNumericData;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Data")
	FItemAssetData ItemAssetData;

	FLinearColor ItemQualityColor;

	bool bIsCopy;
	bool bIsPickup;

	// FUNCTIONS //
	UItemBase();

	void ResetItemFlags();
	
	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * ItemNumericData.Weight; };
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return ItemNumericData.Weight; };
	
	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == ItemNumericData.MaxStackSize; };

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	void SplitStack();

	UFUNCTION(Category = "Item")
	virtual void UseItem(APlayerCharacter* PlayerCharacter);

protected:
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
};
