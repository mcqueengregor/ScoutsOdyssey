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
	UPROPERTY(EditDefaultsOnly, Category=Widgets)
	FVector2D SpeechBubbleOffSet;
public:
	void ShowSpeechBubble() const;
	void HideSpeechBubble() const;
	
protected:
	virtual void BeginPlay() override;

private:
	FVector2D GetPlayerScreenCoordinate(FVector2D Offset) const;
};
