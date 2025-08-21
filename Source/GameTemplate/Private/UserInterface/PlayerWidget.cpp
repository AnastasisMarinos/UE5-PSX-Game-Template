// (C) Anastasis Marinos 2025 //

#include "UserInterface/PlayerWidget.h"
#include "UserInterface/Inventory/ItemDragDropOperation.h"

#include "Components/ProgressBar.h"
#include "Components/WidgetSwitcher.h"

#include "Player/PlayerCharacter.h"
#include "Items/ItemBase.h"


void UPlayerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HealthProgressBar->PercentDelegate.BindUFunction(this, "UpdateHealthProgress");
	HungerProgressBar->PercentDelegate.BindUFunction(this, "UpdateHungerProgress");
	ThirstProgressBar->PercentDelegate.BindUFunction(this, "UpdateThirstProgress");

	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
}

bool UPlayerWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if(PlayerCharacter && ItemDragDrop->SourceItem)
	{
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true;
	}
	return false;
}

void UPlayerWidget::OpenInventory(bool bShouldOpen) // TODO RENAME
{
	if(bShouldOpen)
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
	else
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void UPlayerWidget::OpenStorage(bool bShouldOpen) // TODO RENAME
{
	if(bShouldOpen)
	{
		WidgetSwitcher->SetActiveWidgetIndex(2);
	}
	else
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

float UPlayerWidget::UpdateHealthProgress() const
{
	return PlayerCharacter->Health;
}

float UPlayerWidget::UpdateHungerProgress() const
{
	return PlayerCharacter->Hunger;
}

float UPlayerWidget::UpdateThirstProgress() const
{
	return PlayerCharacter->Thirst;
}
