// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeInteractComponent.h"

#include "Components/AudioComponent.h"
#include "ScoutsOdyssey/InventorySystem/ItemSpawner.h"
#include "ScoutsOdyssey/Animation/CustomSkeletalMeshActor.h"

UTreeInteractComponent::UTreeInteractComponent()
{
	
}

void UTreeInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = Cast<ACustomSkeletalMeshActor>(GetOwner());

	if (OwnerActor)
	{
		OwnerActor->CreateAndAssignDynamicMaterial();
	}

	ValidItemTag = FGameplayTag::RequestGameplayTag(FName("Item.PhilbertsHammer"));
}

void UTreeInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

ECurrentInteraction UTreeInteractComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef)
{
	if (ValidItemTag.MatchesTag(ItemType->ItemTag) && !OwnerActor->GetHasBeenInteractedWith())
	{
		OwnerActor->DisableInteractions();

		FTimerDelegate TreeHitFunction = FTimerDelegate::CreateLambda([=]()
		{			
			if (AcornPropRef)
				AcornPropRef->Destroy();

			if (AcornSpawnerRef)
			{
				AcornSpawnerRef->Spawn();
			}

			UAudioComponent* HammerHitTreeAudio = Cast<UAudioComponent>(
				GetOwner()->GetComponentByClass(UAudioComponent::StaticClass()));

			if (HammerHitTreeAudio)
			{
				HammerHitTreeAudio->Play();
			}
		});

		FTimerHandle TempHandle;
		const USpriteAnimationDataAsset* HammerDA = PlayerRef->GetInteractSpriteDA(ECurrentInteraction::HIT_TREE);
		const float TimeToPlay = HammerDA->InteractionStartIndex * (1.0f / HammerDA->PlaybackFramerate);

		GetWorld()->GetTimerManager().SetTimer(TempHandle, TreeHitFunction, 1.0f,
			false, TimeToPlay);
		
		return ECurrentInteraction::HIT_TREE;
	}

	OnFailToInteract.Broadcast();
	
	return ECurrentInteraction::NO_INTERACTION;
}

void UTreeInteractComponent::DoTask()
{
	if (!OwnerActor->GetHasBeenInteractedWith())
	{
		if (AcornPropRef)
			AcornPropRef->Destroy();
		
		OwnerActor->ToggleAnimationPlayback();
		OwnerActor->DisableInteractions();
		OnTreeDisappear.Broadcast();
	}
}
