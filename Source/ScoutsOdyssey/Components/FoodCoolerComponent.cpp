// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodCoolerComponent.h"

#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"
#include "ScoutsOdyssey/InventorySystem/InventoryComponent.h"

UFoodCoolerComponent::UFoodCoolerComponent()
{
	
}

void UFoodCoolerComponent::BeginPlay()
{
	Super::BeginPlay();

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());

	if (OwnerActor)
	{
		OwnerActor->CreateAndAssignDynamicMaterial();
	}
}

void UFoodCoolerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ECurrentInteraction UFoodCoolerComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef)
{
	// If player interacts with food cooler and 
	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
	if (OwnerActor && OwnerActor->bHasInteractionsRemaining)
	{
		OwnerActor->DisableInteractions();

		UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(
			PlayerRef->GetComponentByClass(UInventoryComponent::StaticClass()));

		if (InventoryComponent && MarshmallowDA)
		{
			InventoryComponent->AddItem(MarshmallowDA, true);
		}
		else if (MarshmallowDA)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
				FString("No reference to marshmallow DA!"));
		}
		else if (!InventoryComponent)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
				FString("No reference to inventory component!"));
		}
		return ECurrentInteraction::COLLECT_MARSHMALLOW;
	}

	return ECurrentInteraction::NO_INTERACTION;
}

void UFoodCoolerComponent::DoTask()
{
	
}
