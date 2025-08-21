// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

struct FInteractableData;

class APlayerCharacter;
class UProgressBar;
class UTextBlock;


UCLASS()
class GAMETEMPLATE_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES //
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget")
	APlayerCharacter* PlayerCharacter;

	// FUNCTIONS //
	void UpdateWidget(const FInteractableData* InteractableData);

protected:
	// PROPERTIES & VARIABLES //
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget")
	UTextBlock* KeyPressText;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget")
	UTextBlock* ActionText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget")
	UProgressBar* InteractionProgressBar;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget")
	float CurrentInteractionDuration;

	// FUNCTIONS //
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(Category = "Interaction Widget")
	float UpdateInteractionProgress();
};
