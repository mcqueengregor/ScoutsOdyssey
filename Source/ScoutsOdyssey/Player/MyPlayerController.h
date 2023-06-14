// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category=Widgets)
	TSubclassOf<UUserWidget> SpeechBubbleAsset;
	UUserWidget* SpeechBubble;
public:
	void ShowSpeechBubble() const;
	void HideSpeechBubble() const;
	
protected:
	virtual void BeginPlay() override;
};
