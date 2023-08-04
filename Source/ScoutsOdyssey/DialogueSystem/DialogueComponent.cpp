// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"
#include "ScoutsOdyssey/Player/MyPlayerController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "DialogueChoiceObject.h"
#include "DialogueMeshActor.h"
#include "Blueprint/UserWidget.h"
#include "SpeechBubbleUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ScoutsOdyssey/LoggingMacros.h"
#include "Engine/World.h"

UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bIsCharacter = false;
	HasTriggered = false;
}

void UDialogueComponent::Click_Implementation(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	// Enable input on click.
	ADialogueMeshActor* DialogueMeshActor = Cast<ADialogueMeshActor>(GetOwner());
	if (DialogueMeshActor)
	{
		DialogueMeshActor->EnableInput(DialogueMeshActor->MyPlayerController);
	}
	else
		LOG_ERROR("Couldn't enable input.");

	Widget_SetUp();
	Delegate_SetUp();

	// Set as -1 when you first start. So won't call speak finish
	SpeakClickCount = -1;
}

bool UDialogueComponent::StartDialogue()
{
	if(HasTriggered && OnlyTriggerOnce)
	{
		return false;
	}

	HasTriggered = true;
	Click_Implementation(nullptr, EKeys::A);

	ADialogueMeshActor* DialogueMeshActor = Cast<ADialogueMeshActor>(GetOwner());
	if (DialogueMeshActor)
	{
		DialogueMeshActor->BehaviorTree_Start(nullptr, EKeys::A);
		return true;
	}
	else
		LOG_ERROR("Couldn't start behavior tree.");

	return false;
}

void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	HasTriggered = false;
}

void UDialogueComponent::Widget_SetUp()
{
	if (PlayerSpeechBubble_WidgetAsset && NPCSpeechBubble_WidgetAsset && NarratorSpeechBubble_WidgetAsset)
	{
		BubbleOne = Cast<
			USpeechBubbleUserWidget>(CreateWidget<UUserWidget>(GetWorld(), PlayerSpeechBubble_WidgetAsset));
		BubbleTwo = Cast<USpeechBubbleUserWidget>(CreateWidget<UUserWidget>(GetWorld(), NPCSpeechBubble_WidgetAsset));
		BubbleNarrator = Cast<USpeechBubbleUserWidget>(
			CreateWidget<UUserWidget>(GetWorld(), NarratorSpeechBubble_WidgetAsset));
		BubbleOne->AddToViewport();
		BubbleTwo->AddToViewport();
		BubbleNarrator->AddToViewport();
		BubbleOne->SetVisibility(ESlateVisibility::Collapsed);
		BubbleTwo->SetVisibility(ESlateVisibility::Collapsed);
		BubbleNarrator->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Speech Bubble Assets not assigned!"));
	}
}

void UDialogueComponent::Delegate_SetUp()
{
	OnDialogueEnd.AddUObject(this, &UDialogueComponent::DialogueEnd_CleanUp);

	ADialogueMeshActor* DialogueMeshActor = Cast<ADialogueMeshActor>(GetOwner());
	if (DialogueMeshActor)
	{
		DialogueMeshActor->OnLeftMouseClick.BindUObject(this, &UDialogueComponent::SpeakFinish);
	}
	else
	{
		LOG_ERROR("Couldn't cast to DialogueMeshActor.");
	}
}

// Issue: given input is bound immediately, this is also invoked every time left mouse is clicked, regardless whether you have started speaking or not. 
void UDialogueComponent::SpeakFinish()
{
	
	if (SpeakClickCount == -1)
	{
		return;
	}
	else if (SpeakClickCount == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpeakTimerHandle);
		SetTextBlockText(CurFullString, *CurTextBlock);
		SpeakClickCount = 1;
	}
	else if (SpeakClickCount == 1)
	{
		CurSpeakString = "";
		SetTextBlockText(CurSpeakString, *CurTextBlock);
		CurChar_Index = 0;
		SpeakClickCount = -1;
		OnSpeakFinish.Broadcast();
	}
	else
	{
		LOG_ERROR("Click Count not -1, 0 or 1!");
	}
}

void UDialogueComponent::ChoiceFinish(const int ReplyIndex)
{
	OnChoiceFinish.Broadcast(ReplyIndex);
}

void UDialogueComponent::SwitchToBubble(const EBubble Bubble) const
{
	ADialogueMeshActor* Owner = Cast<ADialogueMeshActor>(GetOwner());

	if (BubbleOne && BubbleTwo && BubbleNarrator)
	{
		const FVector2D ViewPortSize = AMyPlayerController::GetViewportSize();
		const FVector2D BubbleOneOffSet = ViewPortSize * BubbleOneOffSet_PercentageViewPort;
		const FVector2D BubbleTwoOffSet = ViewPortSize * BubbleTwoOffSet_PercentageViewPort;
		
		auto ClampBubbleWithinScreen = [&](USpeechBubbleUserWidget& BubbleWidget, AActor& Actor, FVector2D OffSet)
		{
			FVector2D BubblePosition = Owner->MyPlayerController->GetActorScreenCoordinate(Actor, OffSet);
			if(BubblePosition.X - 0.1 * ViewPortSize.X < 0)
			{
				BubblePosition.X = 0; //+ 0.05 * ViewPortSize.X;
				BubbleWidget.SetPositionInViewport(BubblePosition);
			} else if (BubblePosition.X + 0.2 * ViewPortSize.X > ViewPortSize.X)
			{
				UImage* Image = Cast<UImage>(BubbleWidget.GetWidgetFromName(TEXT("Image_Tail")));
				Image->SetRenderScale(FVector2D(-1, 1));
				UImage* Image1 = Cast<UImage>(BubbleWidget.GetWidgetFromName(TEXT("Image_Tail1")));
				if(Image1)	
					Image1->SetRenderScale(FVector2D(-1, 1));
				BubblePosition.X = 0.8 * ViewPortSize.X;
				BubbleWidget.SetPositionInViewport(BubblePosition);
			} else
			{
				BubbleWidget.SetPositionInViewport(BubblePosition);
			}
		};
		
		switch (Bubble)
		{
		case EBubble::One:
			ClampBubbleWithinScreen(*BubbleOne, *UGameplayStatics::GetPlayerPawn(this, 0), BubbleOneOffSet);
			//BubbleOne->SetPositionInViewport(Owner->MyPlayerController->GetPlayerScreenCoordinate(BubbleOneOffSet));
			BubbleOne->SetVisibility(ESlateVisibility::Visible);
			BubbleTwo->SetVisibility(ESlateVisibility::Collapsed);
			BubbleNarrator->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case EBubble::Two:
			if (SpeakerTwo)
				ClampBubbleWithinScreen(*BubbleTwo, *SpeakerTwo, BubbleTwoOffSet);
			else
				ClampBubbleWithinScreen(*BubbleTwo, *Owner, BubbleTwoOffSet);	
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
	}
	else
	{
		if (!BubbleOne) UE_LOG(LogTemp, Error, TEXT("Failed to Switch Bubble, Bubble One wasn't created!"));
		if (!BubbleTwo) UE_LOG(LogTemp, Error, TEXT("Failed to Switch Bubble, Bubble Two wasn't created!"));
		if (!BubbleNarrator) UE_LOG(LogTemp, Error, TEXT("Failed to Switch Bubble, Bubble Narrator wasn't created!"));
	}
}

void UDialogueComponent::SwitchBubbleOneState(const EBubbleState BubbleState) const
{
	if (BubbleOne)
	{
		UOverlay* SpeakOverlay = Cast<UOverlay>(BubbleOne->GetWidgetFromName(TEXT("Overlay_Speak")));
		UOverlay* ChoiceOverlay = Cast<UOverlay>(BubbleOne->GetWidgetFromName(TEXT("Overlay_Choice")));

		if (SpeakOverlay && ChoiceOverlay)
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
		}
		else
		{
			if (!SpeakOverlay) UE_LOG(LogTemp, Error, TEXT("SpeakOverlay is Null: %s"), TEXT(__FUNCTION__));
			if (!ChoiceOverlay) UE_LOG(LogTemp, Error, TEXT("ChoiceOverlay is Null: %s"), TEXT(__FUNCTION__));
		}
	}
	else

	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Switch Bubble State, Bubble One wasn't created!"));
	}
}

void UDialogueComponent::SetTextBlockText(const FString& String, UTextBlock& TextWidget) const
{
	TextWidget.SetText(FText::FromString(String));
}

void UDialogueComponent::HideAllBubbles()
{
	BubbleOne->SetVisibility(ESlateVisibility::Collapsed);
	BubbleTwo->SetVisibility(ESlateVisibility::Collapsed);
	BubbleNarrator->SetVisibility(ESlateVisibility::Collapsed);
}

// Was const, now no longer const given TypeNextLetter isn't const
void UDialogueComponent::Speak(const FString& String, const EBubble Bubble, const EVoiceType VoiceType,
                               const int FontSize, const float TalkRate)
{
	if (BubbleOne && BubbleTwo && BubbleNarrator)
	{
		SpeakClickCount = 0;
		CurFullString = String;
		auto SetTimerLambda = [=](const UUserWidget& UserWidget)
		{
			CurTextBlock = Cast<UTextBlock>(UserWidget.GetWidgetFromName(TEXT("TextBlock_Speak")));
			FSlateFontInfo FontInfo = CurTextBlock->Font;
			FontInfo.Size = FontSize;
			CurTextBlock->SetFont(FontInfo);

			GetWorld()->GetTimerManager().SetTimer(SpeakTimerHandle, FTimerDelegate::CreateLambda([=]()
			{
				TypeNextLetter(CurTextBlock, String, VoiceType);
			}), TalkRate, true);
		};

		switch (Bubble)
		{
		case EBubble::One:
			SetTimerLambda(*BubbleOne);
			break;
		case EBubble::Two:
			SetTimerLambda(*BubbleTwo);
			break;
		case EBubble::Narrator:
			SetTimerLambda(*BubbleNarrator);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Speak method took in undefined EBubble value!"));
		}
	}
	else
	{
		if (!BubbleOne) UE_LOG(LogTemp, Error, TEXT("Failed to Speak, Bubble One wasn't created!"));
		if (!BubbleTwo) UE_LOG(LogTemp, Error, TEXT("Failed to Speak, Bubble Two wasn't created!"));
		if (!BubbleNarrator) UE_LOG(LogTemp, Error, TEXT("Failed to Speak, Bubble Narrator wasn't created!"));
	}
}


void UDialogueComponent::Choice(TArray<FText>& Choices)
{
	if (BubbleOne)
	{
		UListView* ListView = Cast<UListView>(BubbleOne->GetWidgetFromName(TEXT("ListView_Choice")));
		ListView->ClearListItems();

		for (int i = 0; i < Choices.Num(); i++)
		{
			UDialogueChoiceObject* DialogueChoiceObject = NewObject<UDialogueChoiceObject>(
				this, UDialogueChoiceObject::StaticClass());
			FText ChoiceText = FText::FromString(Choices[i].ToString());
			DialogueChoiceObject->SetValues(i, ChoiceText);
			ListView->AddItem(DialogueChoiceObject);

			// Bind OnClick. ButtonClick => DialogueChoiceOBJ_OnClick => OnChoiceFinished => Choice_TaskFinish
			DialogueChoiceObject->OnClicked.AddDynamic(this, &UDialogueComponent::ChoiceFinish);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Choice, Bubble One wasn't created!"));
	}
}

void UDialogueComponent::DialogueEnd_CleanUp() const
{
	ADialogueMeshActor* Owner = Cast<ADialogueMeshActor>(GetOwner());

	// Disable Input
	if (Owner)
	{
		Owner->DisableInput(Owner->MyPlayerController);
	}
	else
	{
		LOG_ERROR("Failed to disable Input!");
	}

	// Stop BehaviorTree
	if (Owner->AIController)
		Owner->AIController->GetBrainComponent()->StopLogic("Dialogue End Clean Up");
	else
		UE_LOG(LogTemp, Error, TEXT("Failed to Clean Up AI Controller! NullPointer!"));

	// Destroy all UI
	if (BubbleOne && BubbleTwo && BubbleNarrator)
	{
		BubbleOne->RemoveFromParent();
		BubbleTwo->RemoveFromParent();
		BubbleNarrator->RemoveFromParent();
	}
	else
	{
		if (!BubbleOne) UE_LOG(LogTemp, Error, TEXT("Bubble One Clean up failed! NullPointer!"));
		if (!BubbleTwo) UE_LOG(LogTemp, Error, TEXT("Bubble Two Clean up failed! NullPointer!"));
		if (!BubbleNarrator) UE_LOG(LogTemp, Error, TEXT("Bubble Narrator Clean up failed! NullPointer!"));
	}
}

void UDialogueComponent::TypeNextLetter(UTextBlock* TextBlock, const FString& String, const EVoiceType VoiceType)
{
	if (TextBlock)
	{
		if (CurChar_Index >= String.Len())
		{
			SpeakClickCount = 1;
			GetWorld()->GetTimerManager().ClearTimer(SpeakTimerHandle);
		}
		else
		{
			// Play Sound
			if (String[CurChar_Index] != TCHAR('.') && String[CurChar_Index] != TCHAR(' '))
			{
				//50% chance
				if (CurChar_Index == 0)
					PlayVoiceAtIndex(VoiceType, 0);
				else if (CurChar_Index == String.Len() - 1)
					PlayVoiceAtIndex(VoiceType, LowVoices.Num() - 1);
				else
				{
					if (FMath::RandBool())
						PlayRandomVoice(VoiceType);
				}
			}

			// Set Text
			CurSpeakString += String[CurChar_Index];
			CurChar_Index++;

			if (TextBlock)
				TextBlock->SetText(FText::FromString(CurSpeakString));
			else
				LOG_ERROR("Can't Set Text. TextBlock is null!");
		}
	}
	else
	{
		LOG_ERROR("Can't type next letter. TextBlock is null!");
	}
}

void UDialogueComponent::PlayRandomVoice(EVoiceType VoiceType) const
{
	auto PlayRandomVoiceInArray = [=](TArray<USoundCue*> Voices)
	{
		if (Voices.Num() > 0)
			UGameplayStatics::PlaySound2D(this, Voices[FMath::RandRange(0, Voices.Num() - 1)]);
		else
			LOG_ERROR("No voice inside the voices array!");
	};

	switch (VoiceType)
	{
	case EVoiceType::High:
		PlayRandomVoiceInArray(HighVoices);
		break;
	case EVoiceType::Low:
		PlayRandomVoiceInArray(LowVoices);
		break;
	default:
		LOG_ERROR("Non-existent VoiceType.");
	}
}

void UDialogueComponent::PlayVoiceAtIndex(EVoiceType VoiceType, int Index) const
{
	auto PlayVoiceAtIndex = [&](TArray<USoundCue*> Voices)
	{
		UGameplayStatics::PlaySound2D(this, Voices[Index]);
	};

	switch (VoiceType)
	{
	case EVoiceType::High:
		PlayVoiceAtIndex(HighVoices);
	case EVoiceType::Low:
		PlayVoiceAtIndex(LowVoices);
	}
}


void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}