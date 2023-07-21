// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenhouseInteractComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"

UGreenhouseInteractComponent::UGreenhouseInteractComponent()
{
	SmokeLocalAnimTime = 0.0f;
	bIsPlayingSmokeAnim = false;
}

void UGreenhouseInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());

	if (OwnerActor)
	{
		DynamicMaterial = OwnerActor->CreateAndAssignDynamicMaterial();

		if (SmokeAnimPlaneMesh && SmokeAnimDataAsset)
		{
			UMaterialInterface* MaterialInterface = SmokeAnimPlaneMesh->GetMaterial(0);
			SmokeAnimDynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
			SmokeAnimPlaneMesh->SetMaterial(0, SmokeAnimDynamicMaterial);
			SmokeAnimPlaneMesh->SetVisibility(false);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
				UKismetSystemLibrary::GetDisplayName(this) +
				FString(" missing either material or animation data asset!"));
		}
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
	else if (!bIsPlayingSmokeAnim)
	{
		// TODO: Start "was the greenhouse old?" dialogue
		bIsPlayingSmokeAnim = true;
		
		GetOwner()->GetWorldTimerManager().SetTimer()
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
