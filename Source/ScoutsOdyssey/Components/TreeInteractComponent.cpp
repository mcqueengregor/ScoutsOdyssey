// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeInteractComponent.h"

UTreeInteractComponent::UTreeInteractComponent()
{
	TestValue = 50.0f;
}

void UTreeInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
		FString("Working!"));
}

void UTreeInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTreeInteractComponent::OnInteractWithItem(int32 ItemType, APlayerPawn* PlayerRef)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
		FString("OnInteractWithItem working!"));
}
