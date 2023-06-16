// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeechBubbleUserWidget.h"

FReply USpeechBubbleUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("Clicked On Widget!"));
		OnLeftMouseClick.Execute();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
