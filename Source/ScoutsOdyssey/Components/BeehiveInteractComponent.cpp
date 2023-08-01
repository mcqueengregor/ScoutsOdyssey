// Fill out your copyright notice in the Description page of Project Settings.


#include "BeehiveInteractComponent.h"

#include "ScoutsOdyssey/InventorySystem/InventoryComponent.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"

UBeehiveInteractComponent::UBeehiveInteractComponent()
{
	HoneyBootTag = FGameplayTag::RequestGameplayTag("Item.DeliciousBoot");
	bAreBeesPresent = true;

	static ConstructorHelpers::FObjectFinder<UInventoryItemDataAsset>
		HoneyBootDARef(TEXT("InventoryItemDataAsset'/Game/Blueprints/Inventory/DataAssetItems/DA_DeliciousBoot.DA_DeliciousBoot'"));

	if (HoneyBootDARef.Object)
		HoneyBootDA = HoneyBootDARef.Object;
}

void UBeehiveInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());

	if (OwnerActor)
	{
		OwnerActor->CreateAndAssignDynamicMaterial();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Working!"));
	}
}

void UBeehiveInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ECurrentInteraction UBeehiveInteractComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType,
	APlayerPawn* PlayerRef)
{
	if (ValidItemTag.MatchesTag(ItemType->ItemTag) && !bAreBeesPresent)
	{
		UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(
			PlayerRef->GetComponentByClass(UInventoryComponent::StaticClass()));

		if (InventoryComponent && HoneyBootDA)
		{
			InventoryComponent->RemoveSelectedItem();
			InventoryComponent->AddItem(HoneyBootDA, true);
		}
		
		return ECurrentInteraction::SUCCESS_NO_ANIM;
	}

	OnFailToInteract.Broadcast();
	
	return ECurrentInteraction::NO_INTERACTION;
}

void UBeehiveInteractComponent::DoTask()
{
	
}
