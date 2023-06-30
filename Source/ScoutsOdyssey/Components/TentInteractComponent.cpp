// Fill out your copyright notice in the Description page of Project Settings.


#include "TentInteractComponent.h"
#include "../DialogueSystem/DialogueMeshActor.h"
#include "Kismet/KismetSystemLibrary.h"

UTentInteractComponent::UTentInteractComponent()
{
	CurrentState = FTentState::START;
}

void UTentInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	ADialogueMeshActor* OwnerActor = Cast<ADialogueMeshActor>(GetOwner());
	
	if (OwnerActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
			UKismetSystemLibrary::GetDisplayName(OwnerActor->GetStaticMeshComponent()->GetMaterial(0)));
		
		UMaterialInterface* MatInterface = OwnerActor->GetStaticMeshComponent()->GetMaterial(0);
		DynamicMaterial = UMaterialInstanceDynamic::Create(MatInterface, this);
		OwnerActor->GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
	}
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
	if (ItemType->ItemTag.MatchesTag(ValidItemTag))
	{
		CurrentState = CurrentState == FTentState::START ? FTentState::MIDDLE : FTentState::END;
		UTexture** CurrentTexture = TentStateTextures.Find(CurrentState);
		DynamicMaterial->SetTextureParameterValue("SpriteTexture", *CurrentTexture);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
			FString("Tags don't match!"));
}

void UTentInteractComponent::DoTask()
{
	
}
