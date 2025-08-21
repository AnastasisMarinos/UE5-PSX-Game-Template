// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

struct FItemData;

class UItemBase;


UCLASS()
class GAMETEMPLATE_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// FUNCTIONS //
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 Quantity);
	void InitializeDrop(UItemBase* ItemToDrop, const int32 Quantity);

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

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
	
	void TakePickup(APlayerCharacter* Taker);

	void UpdateInteractableData();

private:
	// PROPERTIES & VARIABLES //
	FInteractableData InstanceInteractableData;
	
	UItemBase* ItemReference;

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
