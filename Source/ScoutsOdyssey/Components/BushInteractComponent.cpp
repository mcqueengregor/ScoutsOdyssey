// Fill out your copyright notice in the Description page of Project Settings.

#include "BushInteractComponent.h"
#include "../Player/PlayerPawn.h"
#include "../DialogueSystem/DialogueMeshActor.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueComponent.h"

UBushInteractComponent::UBushInteractComponent()
{
    LerpT = 0.0f;
    ShrinkDuration = 1.0f;
    bIsShrinking = false;
}

void UBushInteractComponent::BeginPlay()
{
    Super::BeginPlay();

    ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
    
    if (OwnerActor)
    {
        OriginalScale = OwnerActor->GetActorScale();
        OriginalLocation = OwnerActor->GetActorLocation();
    }
}

void UBushInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Increase blend factor over time, if the effect is enabled:
    LerpT += (DeltaTime / ShrinkDuration) * bIsShrinking;
    LerpT = FMath::Clamp(LerpT, 0.0f, 1.0f);

    ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());

    if (OwnerActor)
    {
        OwnerActor->SetActorScale3D(FMath::Lerp(OriginalScale, FinalScale, LerpT));
        OwnerActor->SetActorLocation(FMath::Lerp(OriginalLocation, OriginalLocation + FVector(0.0f, 0.0f, ShrunkLocationOffsetZ), LerpT));
    }
}

ECurrentInteraction UBushInteractComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef)
{
    ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());

    if (OwnerActor)
    {
        UDialogueComponent* DialogueComponent = Cast<UDialogueComponent>(
            OwnerActor->GetComponentByClass(UDialogueComponent::StaticClass()));

        if (DialogueComponent->bIsCharacter)
        {
            DialogueComponent->StartDialogue();
        }
    }

    return ECurrentInteraction::NO_INTERACTION;
}

void UBushInteractComponent::DoTask()
{
    // Start shrinking:
    bIsShrinking = true;
}