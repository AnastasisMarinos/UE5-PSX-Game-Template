// © Anastasis Marinos //

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "PickupBase.generated.h"

struct FItemData;

class UItemBase;

UCLASS()
class GAMETEMPLATE_API APickupBase : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	APickupBase();
	// FUNCTIONS //
	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 Quantity);
	void InitializeDrop(UItemBase* ItemToDrop, const int32 Quantity);

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	// GETTER FUNCTIONS //
	FORCEINLINE UItemBase* GetItemData() { return ItemReference; }

protected:
	// PROPERTIES & VARIABLES //
	UPROPERTY(EditInstanceOnly, Category = "Item Initialization")
	FDataTableRowHandle ItemRowHandle;
	
	UPROPERTY(EditInstanceOnly, Category = "Item Initialization")
	int32 ItemQuantity;
	
	UPROPERTY()
	UStaticMeshComponent* PickupMesh;
	
	// FUNCTIONS //
	virtual void BeginPlay() override;

	virtual void Interact(APlayerCharacter* PlayerCharacter) override;

	void UpdateInteractableData();

	void TakePickup(APlayerCharacter* Taker);

private:
	// PROPERTIES & VARIABLES //
	FInteractableData InstanceInteractableData;

	UPROPERTY()
	UItemBase* ItemReference;

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
