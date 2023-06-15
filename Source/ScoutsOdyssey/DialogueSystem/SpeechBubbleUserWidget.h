// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpeechBubbleUserWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSpeakFinish);
DECLARE_MULTICAST_DELEGATE(FOnChoiceFinish);
DECLARE_MULTICAST_DELEGATE(FOnDialogueEnd);

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API USpeechBubbleUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnSpeakFinish OnSpeakFinish;
	FOnChoiceFinish OnChoiceFinish;
	FOnDialogueEnd OnDialogueEnd;
};
