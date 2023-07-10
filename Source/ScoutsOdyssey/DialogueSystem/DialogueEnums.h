// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DialogueEnums.generated.h"

UENUM(BlueprintType)
enum class EBubble : uint8
{
	One,
	Two,
	Narrator
};

UENUM(BlueprintType)
enum class EBubbleState : uint8
{
	Speak,
	Choice,
};

UENUM(BlueprintType)
enum class EVoiceType : uint8
{
	Low,
	High
};



