// Fill out your copyright notice in the Description page of Project Settings.


#include "TentInteractComponent.h"
#include "../DialogueSystem/DialogueMeshActor.h"

UTentInteractComponent::UTentInteractComponent()
{
	CurrentState = FTentState::START;
}

void UTentInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	
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

void UTentInteractComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
		FString("Tent interact working!"));
}

void UTentInteractComponent::DoTask()
{
	
}
