// (C) Anastasis Marinos 2025 //

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class APlayerCharacter;
class UWidgetSwitcher;
class UProgressBar;


UCLASS()
class GAMETEMPLATE_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// PROPERTIES & VARIABLES //
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	// FUNCTIONS //
	void OpenInventory(bool bShouldOpen);
	void OpenStorage(bool bShouldOpen);

protected:
	// PROPERTIES & VARIABLES //
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Player Stats")
	UProgressBar* HealthProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Player Stats")
	UProgressBar* HungerProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Player Stats")
	UProgressBar* ThirstProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Player Stats")
	UWidgetSwitcher* WidgetSwitcher;

	// FUNCTIONS //
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	UFUNCTION(Category = "Player Stats")
	float UpdateHealthProgress() const;
	
	UFUNCTION(Category = "Player Stats")
	float UpdateHungerProgress() const;
	
	UFUNCTION(Category = "Player Stats")
	float UpdateThirstProgress() const;
};

