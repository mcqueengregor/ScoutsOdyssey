// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpeechBubbleUserWidget.generated.h"


DECLARE_DELEGATE(FOnLeftMouseClick);
/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API USpeechBubbleUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnLeftMouseClick OnLeftMouseClick;
private:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
