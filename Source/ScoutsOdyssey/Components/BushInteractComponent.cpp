// Fill out your copyright notice in the Description page of Project Settings.

#include "BushInteractComponent.h"
#include "../Player/PlayerPawn.h"
#include "../DialogueSystem/DialogueMeshActor.h"
#include "ScoutsOdyssey/DialogueSystem/DialogueComponent.h"

UBushInteractComponent::UBushInteractComponent()
{
    LerpT = 0.0f;
    ShrinkDuration = 1.0f;
    bIsDisappearing = false;
    SmokeLocalAnimTime = 0.0f;
}

void UBushInteractComponent::BeginPlay()
{
    Super::BeginPlay();

    ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
    
    if (OwnerActor)
    {
        OriginalScale = OwnerActor->GetActorScale();
        OriginalLocation = OwnerActor->GetActorLocation();

        if (SmokeAnimDataAsset && SmokePropActorRef)
        {
            SmokePropPlaneMesh = Cast<UStaticMeshComponent>(SmokePropActorRef->GetComponentByClass(
                    UStaticMeshComponent::StaticClass()));
            UMaterialInterface* MaterialInterface = SmokePropPlaneMesh->GetMaterial(0);
            SmokeAnimDynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
            SmokePropPlaneMesh->SetMaterial(0, SmokeAnimDynamicMaterial);
            SmokePropPlaneMesh->SetVisibility(false);
        }
    }
}

void UBushInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Increase blend factor over time, if the effect is enabled:
    LerpT += (DeltaTime / ShrinkDuration) * bIsDisappearing;
    LerpT = FMath::Clamp(LerpT, 0.0f, 1.0f);

    ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());

    if (SmokePropActorRef && bIsDisappearing)
    {
        const int32 NumFrames = SmokeAnimDataAsset->NumSpritesheetColumns *
            SmokeAnimDataAsset->NumSpritesheetRows - SmokeAnimDataAsset->NumEmptyFrames;
        const float AnimDuration = (1.0f / SmokeAnimDataAsset->PlaybackFramerate) * NumFrames;
        SmokeLocalAnimTime += DeltaTime / AnimDuration;

        SmokeAnimDynamicMaterial->SetScalarParameterValue("AnimationLocalTimeNorm", SmokeLocalAnimTime);
        
        if (SmokeLocalAnimTime >= 1.0f)
        {
            SmokeLocalAnimTime = FMath::Clamp(SmokeLocalAnimTime, 0.0f, 0.999f);
            bIsDisappearing = false;
            SmokePropPlaneMesh->SetVisibility(false);
        }
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
    // Start smoke transition:
    bIsDisappearing = true;
    SmokePropPlaneMesh->SetVisibility(true);
    
    ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
    
    FTimerHandle TempHandle;

    FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([=]()
    {
        OwnerActor->SetActorScale3D(FinalScale);
        OwnerActor->SetActorLocation(OriginalLocation + FVector(0.0f, 0.0f, ShrunkLocationOffsetZ));
    });

    float StartDelay = (1.0f / SmokeAnimDataAsset->PlaybackFramerate) * SmokeAnimDataAsset->InteractionStartIndex;
    GetWorld()->GetTimerManager().SetTimer(TempHandle, TimerDelegate, 1.0f, false, StartDelay);
}