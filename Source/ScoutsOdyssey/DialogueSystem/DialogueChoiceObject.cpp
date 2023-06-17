// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueChoiceObject.h"

UDialogueChoiceObject::UDialogueChoiceObject()
{
	
}


void UDialogueChoiceObject::SetValues(const int32 ReplyIndexInput, const FText& ReplyInput)
{
	// Custom constructor implementation
	ReplyIndex = ReplyIndexInput;
	Reply = ReplyInput;
}