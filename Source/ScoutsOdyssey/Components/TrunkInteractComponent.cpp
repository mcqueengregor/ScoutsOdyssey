// Fill out your copyright notice in the Description page of Project Settings.


#include "TrunkInteractComponent.h"

#include "Components/BoxComponent.h"
#include "ScoutsOdyssey/AI/AISquirrelActor.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"
#include "ScoutsOdyssey/InventorySystem/InventoryComponent.h"
#include "ScoutsOdyssey/ItemProps/AcornProp.h"


UTrunkInteractComponent::UTrunkInteractComponent()
{
	HoneyBootItemTag = FGameplayTag::RequestGameplayTag(FName("Item.DeliciousBoot"));
	bAreSquirrelsPresent = true;
}

void UTrunkInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());

	if (OwnerActor)
	{
		OwnerActor->CreateAndAssignDynamicMaterial();
	}
}

void UTrunkInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

ECurrentInteraction UTrunkInteractComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType,
	APlayerPawn* PlayerRef)
{
	if (ValidItemTag.MatchesTag(ItemType->ItemTag) && bAreSquirrelsPresent)
	{
		bAreSquirrelsPresent = false;
		for (const AAISquirrelActor* Squirrel : SquirrelActors)
		{
			// TODO: Squirrel->RunAway();
			// TODO: SquirrelBarrier->Destroy();
		}

		FActorSpawnParameters Parameters = {};
		Parameters.Name = FName("Thrown Acorn Prop");
		Parameters.Owner = PlayerRef;
		Parameters.Instigator = PlayerRef;
		Parameters.bNoFail = true;
		Parameters.bDeferConstruction = true;

		FVector SpawnLocation = PlayerRef->GetActorLocation();
		FRotator SpawnRotator = FRotator(0.0f, 90.0f, 90.0f);

		// TODO: Set timer to spawn acorn w/ impulse on certain frame of "throw acorn" animation!
		AAcornProp* NewAcornProp = Cast<AAcornProp>(GetWorld()->SpawnActor(AcornPropSpawnClass,
			&SpawnLocation, &SpawnRotator, Parameters));

		if (NewAcornProp)
		{
			NewAcornProp->StartDestroyTimer(3.0f);	
			FVector ImpulseDirection = FVector(0.0f, -0.5f, 0.5f);
			const float ImpulseStrength = 1000.0f;
			
			NewAcornProp->BoxColliderComponent->AddImpulse(ImpulseDirection * ImpulseStrength, NAME_None, true);

			if (UInventoryComponent* InventoryComponent =
			Cast<UInventoryComponent>(PlayerRef->GetComponentByClass(UInventoryComponent::StaticClass())))
			{
				InventoryComponent->RemoveSelectedItem();
			}
		}
		
		return ECurrentInteraction::SUCCESS_NO_ANIM;
	}
	else if (HoneyBootItemTag.MatchesTag(ItemType->ItemTag) && !bAreSquirrelsPresent)
	{
		// Tell player to remove honey boot from inventory (current item)
		// Spawn honey boot prop in between trunk sprites (set timer for certain animation frame?)
		// Force player to walk left for X amount of time so that trunk is out of frame, destroy old bear AI and
		// spawn bear actor which is stuck inside trunk
				
		if (UInventoryComponent* InventoryComponent =
			Cast<UInventoryComponent>(PlayerRef->GetComponentByClass(UInventoryComponent::StaticClass())))
		{
			InventoryComponent->RemoveSelectedItem();
		}
		ShowHoneyBoot.Broadcast();

		// TODO: PlayerRef->StartWalkingLeft();
		// TODO: GuardingBear->Destroy();
		// TODO: StuckBear->Show();
		
		return ECurrentInteraction::SUCCESS_NO_ANIM;
	}
	
	return ECurrentInteraction::NO_INTERACTION;
}

void UTrunkInteractComponent::DoTask()
{
	
}
