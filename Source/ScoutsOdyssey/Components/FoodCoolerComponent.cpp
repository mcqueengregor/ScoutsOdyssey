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

		return ECurrentInteraction::COLLECT_MARSHMALLOW;
	}

	return ECurrentInteraction::NO_INTERACTION;
}

void UFoodCoolerComponent::DoTask()
{
	
}

void UFoodCoolerComponent::RemoveMarshmallowsFromCooler()
{
	if (DynamicMaterial && CoolerNoMarshmallowsTex)
	{
		DynamicMaterial->SetTextureParameterValue("SpriteTexture", CoolerNoMarshmallowsTex);
	}
}
