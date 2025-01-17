// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenhouseInteractComponent.h"

#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueComponent.h"
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

		if (SmokeAnimDataAsset && SmokePropActorRef)
		{
			SmokePropPlaneMesh = Cast<UStaticMeshComponent>(SmokePropActorRef->GetComponentByClass(
				UStaticMeshComponent::StaticClass()));
			UMaterialInterface* MaterialInterface = SmokePropPlaneMesh->GetMaterial(0);
			SmokeAnimDynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
			SmokePropPlaneMesh->SetMaterial(0, SmokeAnimDynamicMaterial);
			SmokePropPlaneMesh->SetVisibility(false);
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

	if (bIsPlayingSmokeAnim)
	{
		const int32 NumFrames = SmokeAnimDataAsset->NumSpritesheetColumns *
			SmokeAnimDataAsset->NumSpritesheetRows - SmokeAnimDataAsset->NumEmptyFrames;
		const float AnimDuration = (1.0f / SmokeAnimDataAsset->PlaybackFramerate) * NumFrames;
		SmokeLocalAnimTime += DeltaTime / AnimDuration;
		SmokeAnimDynamicMaterial->SetScalarParameterValue("AnimationLocalTimeNorm", SmokeLocalAnimTime);
		
		if (SmokeLocalAnimTime >= 1.0f)
		{
			SmokeLocalAnimTime = FMath::Clamp(SmokeLocalAnimTime, 0.0f, 0.999f);
			bIsPlayingSmokeAnim = false;
			SmokePropPlaneMesh->SetVisibility(false);
		}
	}
}

ECurrentInteraction UGreenhouseInteractComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType,
                                                                     APlayerPawn* PlayerRef)
{
	if (ItemType->ItemTag.MatchesTag(ValidItemTag) && CurrentState == EGreenhouseState::LOCKED)
	{
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([=]()
		{
			CurrentState = EGreenhouseState::OPEN;
			UTexture* CurrentTexture = *GreenhouseStateTextures.Find(CurrentState);
			DynamicMaterial->SetTextureParameterValue("SpriteTexture", CurrentTexture);
		
			OnGreenHouseUnLocked.Broadcast();

			UAudioComponent* GlassSmashAudio = Cast<UAudioComponent>(
				GetOwner()->GetComponentByClass(UAudioComponent::StaticClass()));

			if (GlassSmashAudio)
			{
				GlassSmashAudio->Play();
			}

			Cast<ADialogueMeshActor>(GetOwner())->DisableInteractions();
		});

		FTimerHandle TempHandle;

		const USpriteAnimationDataAsset* HammerDA = PlayerRef->GetInteractSpriteDA(ECurrentInteraction::SMASH_GREENHOUSE);
		float TimeToPlay = HammerDA->InteractionStartIndex * (1.0f / HammerDA->PlaybackFramerate);

		GetWorld()->GetTimerManager().SetTimer(TempHandle, TimerDelegate, 1.0f,
			false, TimeToPlay);
		
		return ECurrentInteraction::SMASH_GREENHOUSE;
	}
	else if (CurrentState == EGreenhouseState::LOCKED && SmokeAnimDataAsset)
	{
		UDialogueComponent* DialogueComponent = Cast<UDialogueComponent>(GetOwner()->GetComponentByClass(UDialogueComponent::StaticClass()));
		DialogueComponent->StartDialogue();
	}
	
	return ECurrentInteraction::NO_INTERACTION;
}

void UGreenhouseInteractComponent::DoTask()
{
	if (CurrentState == EGreenhouseState::LOCKED)
	{
		PlayGreenhouseAudio.Broadcast();
		
		bIsPlayingSmokeAnim = true;
		SmokePropPlaneMesh->SetVisibility(true);
		float StartDelay = (1.0f / SmokeAnimDataAsset->PlaybackFramerate) * SmokeAnimDataAsset->InteractionStartIndex;
		
		GetOwner()->GetWorldTimerManager().SetTimer(SwitchToOldHandle, this,
			&UGreenhouseInteractComponent::SwitchToOldGreenhouseSprite,	1.0f, false, StartDelay);

		OnGreenHouseUnLocked.Broadcast();
		Cast<ADialogueMeshActor>(GetOwner())->DisableInteractions();
	}
}

void UGreenhouseInteractComponent::SwitchToOldGreenhouseSprite()
{
	CurrentState = EGreenhouseState::OLD;
	UTexture* CurrentTexture = *GreenhouseStateTextures.Find(CurrentState);
	DynamicMaterial->SetTextureParameterValue("SpriteTexture", CurrentTexture);
}
