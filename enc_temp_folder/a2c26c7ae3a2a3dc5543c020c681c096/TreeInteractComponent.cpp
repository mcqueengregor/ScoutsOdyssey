// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeInteractComponent.h"

#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"

UTreeInteractComponent::UTreeInteractComponent()
{
	
}

void UTreeInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTreeInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
	if (OwnerActor)
	{
		OwnerActor->Tick(DeltaTime);
	}
}

void UTreeInteractComponent::OnInteractWithItem(int32 ItemType, APlayerPawn* PlayerRef)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
		FString("TreeInteractComponent::OnInteractWithItem working!"));
}

void UTreeInteractComponent::DoTask()
{
	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
	
	if (OwnerActor)
	{
		OwnerActor->ToggleAnimation();
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("TreeInteractComponent couldn't get reference to owning DialogueMeshActor!"));
	}
}
