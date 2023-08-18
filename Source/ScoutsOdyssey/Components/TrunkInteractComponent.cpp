// Fill out your copyright notice in the Description page of Project Settings.


#include "TrunkInteractComponent.h"

#include <string>

#include "Components/BoxComponent.h"
#include "ScoutsOdyssey/AI/AISquirrelActor.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"
#include "ScoutsOdyssey/InventorySystem/InventoryComponent.h"
#include "ScoutsOdyssey/ItemProps/AcornProp.h"
#include "ScoutsOdyssey/ItemProps/ThrownItemProp.h"

UTrunkInteractComponent::FOnHoneyBootPlaced UTrunkInteractComponent::OnHoneyBootPlaced;

UTrunkInteractComponent::UTrunkInteractComponent()
{
	HoneyBootItemTag = FGameplayTag::RequestGameplayTag(FName("Item.DeliciousBoot"));
	MarshmallowItemTag = FGameplayTag::RequestGameplayTag(FName("Item.Marshmallow"));
	bAreSquirrelsPresent = true;
	NumItemsThrown = 0;
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
	FVector ImpulseDirection = FVector(0.0f, -0.5f, 0.5f);
	const float ImpulseStrength = 1000.0f;
	ImpulseDirection *= ImpulseStrength;
	
	if (ValidItemTag.MatchesTag(ItemType->ItemTag) && bAreSquirrelsPresent)
	{
		PlayerRef->FaceRight();
		
		FTimerDelegate ThrowAcornDelegate = FTimerDelegate::CreateLambda([=]()
		{
			bAreSquirrelsPresent = false;
		
			SquirrelActor->IsAcornThrown = true;
			SquirrelBarrier->DestroyComponent();
			
			SpawnAndThrowProp(AcornPropSpawnClass, ImpulseDirection, PlayerRef);

			OnAcornThrown.Broadcast();
			PlayThrowItemAudio.Broadcast();

			ThrowItemHandle.Invalidate();
		});

		if (ThrowItemHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(ThrowItemHandle);
			GetWorld()->GetTimerManager().ClearTimer(FaceLeftHandle);
		}

		const USpriteAnimationDataAsset* ThrowAcornDA = PlayerRef->GetInteractSpriteDA(ECurrentInteraction::THROW_ACORN);
		
		float StartDelay = (1.0f / ThrowAcornDA->PlaybackFramerate) * ThrowAcornDA->InteractionStartIndex;  
		
		GetWorld()->GetTimerManager().SetTimer(ThrowItemHandle, ThrowAcornDelegate,
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
	else if (MarshmallowItemTag.MatchesTag(ItemType->ItemTag) && bAreSquirrelsPresent)
	{
		PlayerRef->FaceRight();
		
		FTimerDelegate ThrowMarshmallowDelegate = FTimerDelegate::CreateLambda([=]()
		{
			SpawnAndThrowProp(MarshmallowPropSpawnClass, ImpulseDirection, PlayerRef);
			ThrowItemHandle.Invalidate();
			OnMarshmallowThrown.Broadcast();
			PlayThrowItemAudio.Broadcast();
		});

		if (ThrowItemHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(ThrowItemHandle);
			GetWorld()->GetTimerManager().ClearTimer(FaceLeftHandle);
		}

		const USpriteAnimationDataAsset* ThrowAcornDA = PlayerRef->GetInteractSpriteDA(ECurrentInteraction::THROW_ACORN);
		
		float StartDelay = (1.0f / ThrowAcornDA->PlaybackFramerate) * ThrowAcornDA->InteractionStartIndex;  
		
		GetWorld()->GetTimerManager().SetTimer(ThrowItemHandle, ThrowMarshmallowDelegate,
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

	OnFailToInteract.Broadcast();
	
	return ECurrentInteraction::NO_INTERACTION;
}

void UTrunkInteractComponent::DoTask()
{
	
}

void UTrunkInteractComponent::SpawnAndThrowProp(UClass* PropClass, FVector ThrowDirection, APawn* OwningPawnRef)
{
	// Add item index since creating multiple objects of the same name causes a crash:
	FString Name = FString("Thrown Prop ");
	Name.AppendInt(NumItemsThrown);
	
	FActorSpawnParameters Parameters = {};
	Parameters.Name.AppendString(Name);
	Parameters.Owner = OwningPawnRef;											
	Parameters.Instigator = OwningPawnRef;										
	Parameters.bNoFail = true;
	Parameters.bDeferConstruction = true;

	FVector SpawnLocation = OwningPawnRef->GetActorLocation();
	FRotator SpawnRotator = FRotator(0.0f, 90.0f, 90.0f);
	
	AThrownItemProp* NewProp = Cast<AThrownItemProp>(GetWorld()->SpawnActor(PropClass,
		&SpawnLocation, &SpawnRotator, Parameters));

	if (NewProp)
	{
		NewProp->StartDestroyTimer(3.0f);			
		NewProp->BoxColliderComponent->AddImpulse(ThrowDirection, NAME_None, true);

		if (UInventoryComponent* InventoryComponent =
		Cast<UInventoryComponent>(OwningPawnRef->GetComponentByClass(UInventoryComponent::StaticClass())))
		{
			InventoryComponent->RemoveSelectedItem();
		}
	}
	++NumItemsThrown;
}
