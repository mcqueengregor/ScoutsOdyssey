// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeInteractComponent.h"

#include "ScoutsOdyssey/DialogueSystem/DialogueMeshActor.h"

UTreeInteractComponent::UTreeInteractComponent()
{
	
}

void UTreeInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());

	if (OwnerActor)
	{
		LocalAnimTime = 0.0f;
		bAnimFlipFlop = bPlayOnStart;
		
	
		DynamicAnimMaterial = UMaterialInstanceDynamic::Create(
			OwnerActor->GetStaticMeshComponent()->GetMaterial(0), this);
	
		OwnerActor->GetStaticMeshComponent()->SetMaterial(0, DynamicAnimMaterial);
	}
}

void UTreeInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	LocalAnimTime += DeltaTime * (1.0f / AnimationDuration) * static_cast<int32>(bAnimFlipFlop);
	LocalAnimTime = FMath::Clamp(LocalAnimTime, 0.0f, 1.0f);	
	
	if (DynamicAnimMaterial)
		DynamicAnimMaterial->SetScalarParameterValue("AnimationLocalTimeNorm", LocalAnimTime);
}

FCurrentInteraction UTreeInteractComponent::OnInteractWithItem(UInventoryItemDataAsset* ItemType, APlayerPawn* PlayerRef)
{
	return FCurrentInteraction::SUCCESS_NO_ANIM;
}

void UTreeInteractComponent::DoTask()
{
	ToggleAnimation();
}

void UTreeInteractComponent::ToggleAnimation()
{
	bAnimFlipFlop = !bAnimFlipFlop;	
}
