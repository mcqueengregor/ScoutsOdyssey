// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueMeshActor.h"
#include "DialogueComponent.h"
#include "ScoutsOdyssey/Player/MyPlayerController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "DialogueChoiceObject.h"
#include "Blueprint/UserWidget.h"
#include "SpeechBubbleUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ListView.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void ADialogueMeshActor::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	// Spawn AI Controller 
	AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), FVector(0), FRotator(0));
	Clickable_SetUp();
}

void ADialogueMeshActor::BehaviorTree_SetUp()
{
	if(AIController)
	{		
		AIController->RunBehaviorTree(DialogueTree);
		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
		if(Blackboard)
		{
			UDialogueComponent* DialogueComponent =
				Cast<UDialogueComponent>(GetComponentByClass(UDialogueComponent::StaticClass()));
			if (DialogueComponent)
			{
				Blackboard->SetValueAsObject("DialogueComponent", DialogueComponent);
			}
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("BlackBoardComponent is not assigned in AIController!"));	
		}	
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller was not constructed For Dialogue Tree!"));		
	}
}

void ADialogueMeshActor::Clickable_SetUp()
{
	UDialogueComponent* DialogueComponent =
		Cast<UDialogueComponent>(GetComponentByClass(UDialogueComponent::StaticClass()));

	UStaticMeshComponent* MyStaticMeshComponent =
		Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	
	if (DialogueComponent && MyStaticMeshComponent)
		MyStaticMeshComponent->OnClicked.AddDynamic(DialogueComponent, &UDialogueComponent::Click_Implementation);
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dialogue mesh actor OnClicked delegate wasn't set up (DialogueComponent was nullptr)"));
	}
}
