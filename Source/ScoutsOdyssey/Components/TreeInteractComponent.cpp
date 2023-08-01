// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeInteractComponent.h"

#include "Components/AudioComponent.h"
#include "ScoutsOdyssey/InventorySystem/ItemSpawner.h"
#include "ScoutsOdyssey/Animation/CustomSkeletalMeshActor.h"
#include "UnrealAudio/Private/UnrealAudioBuffer.h"

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
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow,
				FString("Sound played!"));
		}
		
		return ECurrentInteraction::SUCCESS_NO_ANIM;
	}
	
	DoTask();
	
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
