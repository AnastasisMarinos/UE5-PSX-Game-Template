// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"


UENUM()
enum class EItemQuality : uint8
{
	Casual UMETA(DisplayName = "Casual"),
	Premium UMETA(DisplayName = "Premium"),
	Luxurious UMETA(DisplayName = "Luxurious"),
};

UENUM()
enum class EItemType : uint8
{
	Consumable UMETA(DisplayName = "Consumable"),
	Weapon UMETA(DisplayName = "Weapon"),
	Key UMETA(DisplayName = "Key"),
};

USTRUCT()
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float Condition;
	
	UPROPERTY(EditAnywhere)
	float SellValue;
	
	UPROPERTY(EditAnywhere)
	float PurchaseValue;

	UPROPERTY(EditAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	float Hunger;
	
	UPROPERTY(EditAnywhere)
	float Thirst;
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	FText Name;
	
	UPROPERTY(EditAnywhere)
	FText Description;
	
	UPROPERTY(EditAnywhere)
	FText InteractionText;
	
	UPROPERTY(EditAnywhere)
	FText UsageText;
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float Weight;
	
	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;
	
	UPROPERTY(EditAnywhere)
	bool bIsStackable;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	USoundBase* InteractionSound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* UsageSound;

	UPROPERTY(EditAnywhere)
	UAnimMontage* InteractionAnim;

	UPROPERTY(EditAnywhere)
	UAnimMontage* UsageAnim;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData ItemTextData;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData ItemNumericData;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData ItemAssetData;
};
