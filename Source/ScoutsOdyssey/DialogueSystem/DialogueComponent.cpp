// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"
#include "ScoutsOdyssey/Player/MyPlayerController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDialogueComponent::Click_Implementation(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) 
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked! From Dialogue Component"));
	if(MyPlayerController)
	{
		MyPlayerController->ShowSpeechBubble();	
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("MyPlayerController from DialogueComponent is null"));
	}
	
}


void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ClickableSetUp();
}

void UDialogueComponent::ClickableSetUp()
{
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

