// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDialogueComponent::Click_Implementation(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) 
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked!"));	
}


void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UStaticMeshComponent* OwnerMesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if(OwnerMesh)
	{
		OwnerMesh->OnClicked.AddDynamic(this, &UDialogueComponent::Click_Implementation);	
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("cast failed!"));	
	}
}


void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

