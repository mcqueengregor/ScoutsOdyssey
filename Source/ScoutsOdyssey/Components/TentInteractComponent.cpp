// Fill out your copyright notice in the Description page of Project Settings.


#include "TentInteractComponent.h"

#include <string>

#include "../DialogueSystem/DialogueMeshActor.h"
#include "Chaos/ChaosPerfTest.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ScoutsOdyssey/LoggingMacros.h"

int UTentInteractComponent::NumberOfTents = 0;

UTentInteractComponent::UTentInteractComponent()
{
	CurrentState = ETentState::START;
}

void UTentInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	NumberOfTents = 0;

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
	
	if (OwnerActor)
	{
		UMaterialInterface* MatInterface = OwnerActor->GetStaticMeshComponent()->GetMaterial(0);
		DynamicMaterial = OwnerActor->CreateAndAssignDynamicMaterial();
		
		OriginalLocation = OwnerActor->GetActorLocation();
		OriginalScaleMultiplier = OwnerActor->GetActorScale().X;
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
				static_cast<float>(TentStateTextures.Find(ETentState::START)->TentStateTexture->GetSizeX());
			Texture.Value.TextureScale.Z = 1.0f;
		}
	}

	ValidItemTag = FGameplayTag::RequestGameplayTag(FName("Item.PhilbertsHammer"));
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

ECurrentInteraction UTentInteractComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef)
{
    if (ItemType->ItemTag.MatchesTag(ValidItemTag) && CurrentState != ETentState::END)
    {
	    FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([=]()
		{
			CurrentState = CurrentState == ETentState::START ? ETentState::MIDDLE : ETentState::END;
			UTexture* CurrentTexture = TentStateTextures.Find(CurrentState)->TentStateTexture;
			DynamicMaterial->SetTextureParameterValue("SpriteTexture", CurrentTexture);

			if (ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner()))
			{
				OwnerActor->GetStaticMeshComponent()->SetRelativeScale3D(
					TentStateTextures.Find(CurrentState)->TextureScale * OriginalScaleMultiplier);

				// TODO: Make this proportional to differences in resolution between tent sprites:
				OwnerActor->SetActorLocation(OriginalLocation + (FVector(0.0f, 0.0f, 30.0f) * OriginalScaleMultiplier));
				
				if (CurrentState == ETentState::END)
				{
					NumberOfTents++;
	    			Cast<ADialogueMeshActor>(GetOwner())->DisableInteractions();
					
					if(NumberOfTents == RequiredNumberOfTents)
					{
						OnAllTentBuilt.Broadcast();
					}
				}

				UAudioComponent* HammerHitPegAudio = Cast<UAudioComponent>(
					GetOwner()->GetComponentByClass(UAudioComponent::StaticClass()));
				if (HammerHitPegAudio)
				{
					HammerHitPegAudio->Play();
				}
			}
	    	InteractionTimerHandle.Invalidate();

		});

		if (!InteractionTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
		}
    	
		const USpriteAnimationDataAsset* HammerDA = PlayerRef->GetInteractSpriteDA(ECurrentInteraction::HIT_TREE);
    	float TimeToPlay = HammerDA->InteractionStartIndex * (1.0f / HammerDA->PlaybackFramerate);
    	
    	GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle, TimerDelegate, 1.0f,
    		false, TimeToPlay);
    		
    	return ECurrentInteraction::HIT_TREE;
    }
	
	// if interaction fails
	OnFailToInteract.Broadcast();
    return ECurrentInteraction::NO_INTERACTION;
}

void UTentInteractComponent::DoTask()
{
	
}
