// Fill out your copyright notice in the Description page of Project Settings.


#include "TentInteractComponent.h"

#include <string>

#include "../DialogueSystem/DialogueMeshActor.h"
#include "Chaos/ChaosPerfTest.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ScoutsOdyssey/LoggingMacros.h"

int UTentInteractComponent::NumberOfTents = 0;

UTentInteractComponent::UTentInteractComponent()
{
	CurrentState = FTentState::START;
}

void UTentInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	NumberOfTents = 0;

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
	
	if (OwnerActor)
	{
		UMaterialInterface* MatInterface = OwnerActor->GetStaticMeshComponent()->GetMaterial(0);
		DynamicMaterial = UMaterialInstanceDynamic::Create(MatInterface, this);
		OwnerActor->GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
		OwnerActor->DynamicMaterial = DynamicMaterial;
		
		OriginalLocation = OwnerActor->GetActorLocation();
		OriginalScaleMultipler = OwnerActor->GetActorScale().X;
	}

	// Store scales of each tent texture relative to the "supplies" texture, so that sprite plane
	// can change scale without warping the sprite applied to it:
	for (auto& Texture : TentStateTextures)
	{
		if (Texture.Value.TentStateTexture)
		{
			Texture.Value.TextureScale = FVector(
				Texture.Value.TentStateTexture->GetSizeX(),
				Texture.Value.TentStateTexture->GetSizeY(),
				0.0f);
			Texture.Value.TextureScale.Normalize();

			// Force x-component to 1 without warping texture.
			Texture.Value.TextureScale /= Texture.Value.TextureScale.X;

			// Adjust scale relative to "supplies" texture:
			Texture.Value.TextureScale *= Texture.Value.TentStateTexture->GetSizeX() /
				static_cast<float>(TentStateTextures.Find(FTentState::START)->TentStateTexture->GetSizeX());
			Texture.Value.TextureScale.Z = 1.0f;
		}
	}
}

void UTentInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
	if (OwnerActor)
	{
		OwnerActor->Tick(DeltaTime);
	}
}

FCurrentInteraction UTentInteractComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef)
{
    if (ItemType->ItemTag.MatchesTag(ValidItemTag) && CurrentState != FTentState::END)
    {
        CurrentState = CurrentState == FTentState::START ? FTentState::MIDDLE : FTentState::END;
        UTexture* CurrentTexture = TentStateTextures.Find(CurrentState)->TentStateTexture;
        DynamicMaterial->SetTextureParameterValue("SpriteTexture", CurrentTexture);

        if (ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner()))
        {
            OwnerActor->GetStaticMeshComponent()->SetRelativeScale3D(
                TentStateTextures.Find(CurrentState)->TextureScale * OriginalScaleMultipler);

            // TODO: Make this proportional to differences in resolution between tent sprites:
            OwnerActor->SetActorLocation(OriginalLocation + (FVector(0.0f, 0.0f, 30.0f) *  OriginalScaleMultipler));

            // NOTE FOR HAO: Add logic for keeping track of the number of tents fully put up here!
            if (CurrentState == FTentState::END)
            {
	            NumberOfTents++;
            	PRINT(FString::FromInt(NumberOfTents));
            	if(NumberOfTents == RequiredNumberOfTents)
            	{
            		OnAllTentBuilt.Broadcast();
            	} 
            }

            return FCurrentInteraction::SUCCESS_NO_ANIM;
        }
    }

	// if interaction fails
	OnFailToInteract.Broadcast();
    return FCurrentInteraction::NO_INTERACTION;
}

void UTentInteractComponent::DoTask()
{
	
}
