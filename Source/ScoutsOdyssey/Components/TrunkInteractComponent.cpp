// Fill out your copyright notice in the Description page of Project Settings.


#include "TrunkInteractComponent.h"
#include "Components/BoxComponent.h"
#include "ScoutsOdyssey/AI/AISquirrelActor.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"
#include "ScoutsOdyssey/InventorySystem/InventoryComponent.h"
#include "ScoutsOdyssey/ItemProps/AcornProp.h"

UTrunkInteractComponent::FOnHoneyBootPlaced UTrunkInteractComponent::OnHoneyBootPlaced;

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

void UTrunkInteractComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// needs to clear its invocation list due to static delegate
	OnHoneyBootPlaced.Clear();
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
		FTimerDelegate ThrowAcornDelegate = FTimerDelegate::CreateLambda([=]()
		{
			bAreSquirrelsPresent = false;
		
			SquirrelActor->IsAcornThrown = true;
			SquirrelBarrier->DestroyComponent();

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

			OnAcornThrown.Broadcast();

			ThrowAcornHandle.Invalidate();
		});

		if (ThrowAcornHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(ThrowAcornHandle);
			GetWorld()->GetTimerManager().ClearTimer(FaceLeftHandle);
		}

		const USpriteAnimationDataAsset* ThrowAcornDA = PlayerRef->GetInteractSpriteDA(ECurrentInteraction::THROW_ACORN);
		
		float StartDelay = (1.0f / ThrowAcornDA->PlaybackFramerate) * ThrowAcornDA->InteractionStartIndex;  
		
		GetWorld()->GetTimerManager().SetTimer(ThrowAcornHandle, ThrowAcornDelegate,
			1.0f, false, StartDelay);

		// Start timer for forcing the player to look left after animation is complete:
		FTimerDelegate FaceLeftDelegate = FTimerDelegate::CreateLambda([=]()
		{
			PlayerRef->FaceLeft();
		});

		GetWorld()->GetTimerManager().SetTimer(FaceLeftHandle, FaceLeftDelegate, 1.0f, false,
			(1.0f / ThrowAcornDA->PlaybackFramerate) *
			((ThrowAcornDA->NumSpritesheetColumns * ThrowAcornDA->NumSpritesheetRows) - ThrowAcornDA->NumEmptyFrames));
		
		return ECurrentInteraction::THROW_ACORN;
	}
	else if (HoneyBootItemTag.MatchesTag(ItemType->ItemTag) && !bAreSquirrelsPresent)
	{
		if (UInventoryComponent* InventoryComponent =
			Cast<UInventoryComponent>(PlayerRef->GetComponentByClass(UInventoryComponent::StaticClass())))
		{
			InventoryComponent->RemoveSelectedItem();
		}
		ShowHoneyBoot.Broadcast();

		// Player Running Left, BearAI transition subscribes to it.
		OnHoneyBootPlaced.Broadcast();

		Cast<ADialogueMeshActor>(GetOwner())->DisableInteractions();
		
		return ECurrentInteraction::SUCCESS_NO_ANIM;
	}

	OnFailToInteract.Broadcast();
	
	return ECurrentInteraction::NO_INTERACTION;
}

void UTrunkInteractComponent::DoTask()
{
	
}


