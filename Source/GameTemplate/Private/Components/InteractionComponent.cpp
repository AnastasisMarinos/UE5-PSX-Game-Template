// (C) Anastasis Marinos 2025 //

#include "Components/InteractionComponent.h"

#include "Player/PlayerHUD.h"
#include "Player/PlayerCharacter.h"

#include "Interfaces/InteractionInterface.h"
#include "Kismet/GameplayStatics.h"


UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1;

	Character = Cast<APlayerCharacter>(GetOwner());

	InteractionCheckDistance = 225.0f;
}

// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	PerformInteractionCheck();
}

void UInteractionComponent::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector Loc;
	FRotator Rot;
	FHitResult Hit;
	
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * 250);

	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Character);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams))
	{
		if(Hit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			if(Hit.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(Hit.GetActor());
				return;
			}

			if(Hit.GetActor() == InteractionData.CurrentInteractable) // When Player keeps looking at the same interactable.
			{
				return; 
			}
		}
	}

	//DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);
	
	NoInteractableFound();
}

void UInteractionComponent::FoundInteractable(AActor* NewInteractable)
{
	if(IsInteracting())
	{
		EndInteract();
	}

	if(InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	
	TargetInteractable->BeginFocus();
}

void UInteractionComponent::NoInteractableFound()
{
	if(IsInteracting())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if(InteractionData.CurrentInteractable)
	{
		if(IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus(); // Might crash if we don't end before destroying.
		}

		HUD->HideInteractionWidget();
		
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void UInteractionComponent::BeginInteract()
{
	// Verify nothing has changed with the interactable state since beginning interaction.
	PerformInteractionCheck();

	if(InteractionData.CurrentInteractable)
	{
		if(IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if(FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_Interaction,
					this, &UInteractionComponent::Interact,
					TargetInteractable->InteractableData.InteractionDuration, false);
			}
		}
	}
}

void UInteractionComponent::EndInteract()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);

	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void UInteractionComponent::Interact()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction);

	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(Character);
	}
}

void UInteractionComponent::UpdateInteractionWidget() const
{
	if(IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

