// Fill out your copyright notice in the Description page of Project Settings.


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

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDialogueComponent::Click_Implementation(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) 
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked! From Dialogue Component"));

	Widget_SetUp();
	Delegate_SetUp();
	BehaviorTree_SetUp();
}


void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	// Spawn AI Controller 
	AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), FVector(0), FRotator(0));
	Clickable_SetUp();
}

void UDialogueComponent::Clickable_SetUp()
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

void UDialogueComponent::Widget_SetUp()
{
	if(PlayerSpeechBubble_WidgetAsset && NPCSpeechBubble_WidgetAsset && NarratorSpeechBubble_WidgetAsset)
	{
		BubbleOne = Cast<USpeechBubbleUserWidget>(CreateWidget<UUserWidget>(GetWorld(), PlayerSpeechBubble_WidgetAsset));
		BubbleTwo = Cast<USpeechBubbleUserWidget>(CreateWidget<UUserWidget>(GetWorld(), NPCSpeechBubble_WidgetAsset));
		BubbleNarrator = Cast<USpeechBubbleUserWidget>(CreateWidget<UUserWidget>(GetWorld(), NarratorSpeechBubble_WidgetAsset)); 
		BubbleOne->AddToViewport();
		BubbleTwo->AddToViewport();
		BubbleNarrator->AddToViewport();
		BubbleOne->SetVisibility(ESlateVisibility::Collapsed);
		BubbleTwo->SetVisibility(ESlateVisibility::Collapsed);
		BubbleNarrator->SetVisibility(ESlateVisibility::Collapsed);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Speech Bubble Assets not assigned!"));
	}
}

void UDialogueComponent::BehaviorTree_SetUp()
{
	if(AIController)
	{
		AIController->RunBehaviorTree(DialogueTree);
		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
		if(Blackboard)
		{
			Blackboard->SetValueAsObject("DialogueComponent", this);
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("BlackBoardComponent is not assigned in AIController!"));	
		}	
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller was not constructed For Dialogue Tree!"));		
	}

}

void UDialogueComponent::Delegate_SetUp()
{
	OnDialogueEnd.AddUObject(this, &UDialogueComponent::DialogueEnd_CleanUp);

	// LeftMouseClick Delegates, should always trigger, always on the same UI.
	BubbleOne->OnLeftMouseClick.BindUObject(this, &UDialogueComponent::SpeakFinish);
	BubbleTwo->OnLeftMouseClick.BindUObject(this, &UDialogueComponent::SpeakFinish);
	BubbleNarrator->OnLeftMouseClick.BindUObject(this, &UDialogueComponent::SpeakFinish);
}


void UDialogueComponent::SpeakFinish() const
{
	UE_LOG(LogTemp, Warning, TEXT("SpeakFinish Broadcasted!"));
	OnSpeakFinish.Broadcast();
}

void UDialogueComponent::ChoiceFinish(const int ReplyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("ChoiceFinish Broadcasted!"));
	OnChoiceFinish.Broadcast(ReplyIndex);	
}


void UDialogueComponent::SwitchToBubble(const EBubble Bubble) const
{
	if(BubbleOne && BubbleTwo && BubbleNarrator)
	{
		switch (Bubble)
		{
		case EBubble::One:
			BubbleOne->SetPositionInViewport(MyPlayerController->GetPlayerScreenCoordinate(BubbleOneOffSet));
			BubbleOne->SetVisibility(ESlateVisibility::Visible);
			BubbleTwo->SetVisibility(ESlateVisibility::Collapsed);
			BubbleNarrator->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case EBubble::Two:
			BubbleTwo->SetPositionInViewport(MyPlayerController->GetActorScreenCoordinate(*GetOwner(), BubbleTwoOffSet));	
			BubbleOne->SetVisibility(ESlateVisibility::Collapsed);
			BubbleTwo->SetVisibility(ESlateVisibility::Visible);
			BubbleNarrator->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case EBubble::Narrator:
			BubbleOne->SetVisibility(ESlateVisibility::Collapsed);
			BubbleTwo->SetVisibility(ESlateVisibility::Collapsed);
			BubbleNarrator->SetVisibility(ESlateVisibility::Visible);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Switch to bubble took in undefined Bubble type!"));	
		}	
	} else
	{
		if(!BubbleOne) UE_LOG(LogTemp, Error, TEXT("Failed to Switch Bubble, Bubble One wasn't created!"));
		if(!BubbleTwo) UE_LOG(LogTemp, Error, TEXT("Failed to Switch Bubble, Bubble Two wasn't created!"));
		if(!BubbleNarrator) UE_LOG(LogTemp, Error, TEXT("Failed to Switch Bubble, Bubble Narrator wasn't created!"));
	}
}

void UDialogueComponent::SwitchBubbleOneState(const EBubbleState BubbleState) const
{
	if(BubbleOne)
	{
		UOverlay* SpeakOverlay = Cast<UOverlay>(BubbleOne->GetWidgetFromName(TEXT("Overlay_Speak")));	
		UOverlay* ChoiceOverlay = Cast<UOverlay>(BubbleOne->GetWidgetFromName(TEXT("Overlay_Choice")));		

		if(SpeakOverlay && ChoiceOverlay)
		{
			switch (BubbleState)
			{
			case EBubbleState::Speak:
				SpeakOverlay->SetVisibility(ESlateVisibility::Visible);
				ChoiceOverlay->SetVisibility(ESlateVisibility::Collapsed);
				break;
			case EBubbleState::Choice:
				SpeakOverlay->SetVisibility(ESlateVisibility::Collapsed);
				ChoiceOverlay->SetVisibility(ESlateVisibility::Visible);
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("Switch bubble state took in undefined Bubble state!"));		
			}
		} else {
			if(!SpeakOverlay) UE_LOG(LogTemp, Error, TEXT("SpeakOverlay is Null: %s"), TEXT(__FUNCTION__));
			if(!ChoiceOverlay) UE_LOG(LogTemp, Error, TEXT("ChoiceOverlay is Null: %s"), TEXT(__FUNCTION__));
		}
	} else
		
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Switch Bubble State, Bubble One wasn't created!"));	
	}
}

void UDialogueComponent::SetTextBlockText(const FText& Text, const UUserWidget& Parent) const
{
	UTextBlock* TextWidget = Cast<UTextBlock>(Parent.GetWidgetFromName(TEXT("TextBlock_Speak")));
	if(TextWidget)
		TextWidget->SetText(Text);
	else
		UE_LOG(LogTemp, Error, TEXT("Speak failed, Bubble One TextWidget not found!"))	
}


void UDialogueComponent::Speak(const FText& Text, const EBubble Bubble) const
{
	if(BubbleOne && BubbleTwo && BubbleNarrator)
	{
		switch (Bubble)
		{
		case EBubble::One:
			SetTextBlockText(Text, *BubbleOne);	
			break;
		case EBubble::Two:
			SetTextBlockText(Text, *BubbleTwo);		
			break;
		case EBubble::Narrator:
			SetTextBlockText(Text, *BubbleNarrator);	
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Speak method took in undefined EBubble value!"));	
		}	
	} else
	{
		if(!BubbleOne) UE_LOG(LogTemp, Error, TEXT("Failed to Speak, Bubble One wasn't created!"));
		if(!BubbleTwo) UE_LOG(LogTemp, Error, TEXT("Failed to Speak, Bubble Two wasn't created!"));
		if(!BubbleNarrator) UE_LOG(LogTemp, Error, TEXT("Failed to Speak, Bubble Narrator wasn't created!"));
	}	
}

void UDialogueComponent::Choice(TArray<FText>& Choices)
{
	if(BubbleOne)
	{
		UListView* ListView = Cast<UListView>(BubbleOne->GetWidgetFromName(TEXT("ListView_Choice")));
		
		for(int i = 0; i < Choices.Num(); i++)
		{
			UDialogueChoiceObject* DialogueChoiceObject = NewObject<UDialogueChoiceObject>(this, UDialogueChoiceObject::StaticClass());
			DialogueChoiceObject->SetValues(i, Choices[i]);
			ListView->AddItem(DialogueChoiceObject);

			// Bind OnClick. ButtonClick => DialogueChoiceOBJ_OnClick => OnChoiceFinished => Choice_TaskFinish
			DialogueChoiceObject->OnClicked.AddDynamic(this, &UDialogueComponent::ChoiceFinish);
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Choice, Bubble One wasn't created!"));	
	} 
}

void UDialogueComponent::DialogueEnd_CleanUp() const
{
	// Stop BehaviorTree
	 if(AIController)
	 	AIController->GetBrainComponent()->StopLogic("Dialogue End Clean Up");
	 else
	 	UE_LOG(LogTemp, Error, TEXT("Failed to Clean Up AI Controller! NullPointer!"));	
	
	// Destroy all UI
	if(BubbleOne && BubbleTwo && BubbleNarrator)
	{
		BubbleOne->RemoveFromParent();
		BubbleTwo->RemoveFromParent();
		BubbleNarrator->RemoveFromParent();	
	} else
	{
		if(!BubbleOne) UE_LOG(LogTemp, Error, TEXT("Bubble One Clean up failed! NullPointer!"));
		if(!BubbleTwo) UE_LOG(LogTemp, Error, TEXT("Bubble Two Clean up failed! NullPointer!"));
		if(!BubbleNarrator) UE_LOG(LogTemp, Error, TEXT("Bubble Narrator Clean up failed! NullPointer!"));
	}
}


void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

