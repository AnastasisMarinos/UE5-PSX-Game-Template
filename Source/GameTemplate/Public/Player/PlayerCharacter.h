// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInteractionComponent;
class UInventoryComponent;
class APlayerHUD;
class UItemBase;


UCLASS()
class GAMETEMPLATE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY()
	APlayerHUD* HUD;

	float Health = 1.0f;
	float Hunger = 0.1f;
	float Thirst = 0.1f;
	
	// FUNCTIONS //
	APlayerCharacter();

	void UpdateInteractionWidget() const;
	void InteractWithItem(USoundBase* InputUseSound, UAnimMontage* InputUseAnim);
	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);
	void StoreItem(UItemBase* ItemToStore, const int32 QuantityToStore);
	void UseItem(USoundBase* InputUseSound, UAnimMontage* InputUseAnim);

	FORCEINLINE UInventoryComponent* GetInventory() const { return InventoryComponent; }

protected:
	// PROPERTIES & VARIABLES //
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* CameraArm;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;
	
	// FUNCTIONS //
	virtual void BeginPlay() override;
	
};

