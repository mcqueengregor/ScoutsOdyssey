// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "Blueprint/UserWidget.h"

void AMyPlayerController::ShowSpeechBubble() const
{
	if(SpeechBubble)
	{
		SpeechBubble->SetPositionInViewport(GetPlayerScreenCoordinate(SpeechBubbleOffSet));
		SpeechBubble->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Speech Bubble Widget is null!"));	
	}
}

void AMyPlayerController::HideSpeechBubble() const
{
	if(SpeechBubble)
		SpeechBubble->SetVisibility(ESlateVisibility::Collapsed);
	else
		UE_LOG(LogTemp, Warning, TEXT("Speech Bubble Widget is null!"));
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(SpeechBubbleAsset)
	{
		SpeechBubble = CreateWidget<UUserWidget>(this, SpeechBubbleAsset);
		SpeechBubble->AddToViewport();
		HideSpeechBubble();	
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Speech Bubble Asset not assigned!"));
	}
}

FVector2D AMyPlayerController::GetPlayerScreenCoordinate(FVector2D Offset) const
{
	const FVector ActorLocation = GetPawn()->GetActorLocation();
	FVector2D ScreenLocation;
	ProjectWorldLocationToScreen(ActorLocation, ScreenLocation);

	return ScreenLocation + Offset;
}
