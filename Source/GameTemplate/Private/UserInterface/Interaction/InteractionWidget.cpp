// (C) Anastasis Marinos 2025 //

#include "UserInterface/Interaction/InteractionWidget.h"

#include "Interfaces/InteractionInterface.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f;
}

void UInteractionWidget::UpdateWidget(const FInteractableData* InteractableData)
{
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::Item:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if(InteractableData->Quantity < 2)
		{
			ActionText->SetText(FText::Format(
			FText::FromString("to {0} {1}"), InteractableData->Action, InteractableData->Name));
		}
		else
		{
			ActionText->SetText(FText::Format(
			FText::FromString("to {0} x{1} {2}"), InteractableData->Action, InteractableData->Quantity, InteractableData->Name));
		}
		break;
		
	case EInteractableType::NPC:
		break;

	case EInteractableType::Vehicle:
		break;
		
	case EInteractableType::Storage:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		ActionText->SetText(FText::Format(
			FText::FromString("to {0} {1}"), InteractableData->Action, InteractableData->Name));
		break;
	case EInteractableType::Environment:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		ActionText->SetText(FText::Format(
			FText::FromString("to {0} {1}"), InteractableData->Action, InteractableData->Name));
		break;
	default: ;
	}
}

float UInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}
