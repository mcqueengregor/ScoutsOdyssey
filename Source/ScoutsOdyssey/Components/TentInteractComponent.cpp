// Fill out your copyright notice in the Description page of Project Settings.


#include "TentInteractComponent.h"

UTentInteractComponent::UTentInteractComponent()
{
	
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
	
}

void UTentInteractComponent::DoTask()
{
	
}
