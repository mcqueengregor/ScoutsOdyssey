// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenhouseInteractComponent.h"

#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"

void UGreenhouseInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());

	if (OwnerActor)
	{
		DynamicMaterial = OwnerActor->CreateAndAssignDynamicMaterial();
	}
	
	ValidItemTag = FGameplayTag::RequestGameplayTag(FName("Item.PhilbertsHammer"));
}

void UGreenhouseInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

ECurrentInteraction UGreenhouseInteractComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType,
                                                                     APlayerPawn* PlayerRef)
{
	if (ItemType->ItemTag.MatchesTag(ValidItemTag) && CurrentState == EGreenhouseState::LOCKED)
	{
		CurrentState = EGreenhouseState::OPEN;
		UTexture* CurrentTexture = *GreenhouseStateTextures.Find(CurrentState);
		DynamicMaterial->SetTextureParameterValue("SpriteTexture", CurrentTexture);
		
		return ECurrentInteraction::SUCCESS_NO_ANIM;
	}
	else
	{
		// TODO: Start "was the greenhouse old?" dialogue
	}
	
	return ECurrentInteraction::NO_INTERACTION;
}

void UGreenhouseInteractComponent::DoTask()
{
	if (CurrentState == EGreenhouseState::LOCKED)
	{
		CurrentState = EGreenhouseState::OLD;
		UTexture* CurrentTexture = *GreenhouseStateTextures.Find(CurrentState);
		DynamicMaterial->SetTextureParameterValue("SpriteTexture", CurrentTexture);
	}
}
