// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InteractionInterface.h"

#include "InteractionComponent.generated.h"

class APlayerCharacter;
class APlayerHUD;
class UItemBase;
class UInventoryComponent;


// Include in inventory system //
USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f) { }

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMETEMPLATE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES //
	UPROPERTY()
	APlayerHUD* HUD;

	// FUNCTIONS //
	UInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE bool IsInteracting() const { return GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Interaction); }

	void UpdateInteractionWidget() const;

	void BeginInteract();
	void EndInteract();

protected:
	// PROPERTIES & VARIABLES //
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	APlayerCharacter* Character;
	
	// FUNCTIONS //
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void Interact();
	
};
