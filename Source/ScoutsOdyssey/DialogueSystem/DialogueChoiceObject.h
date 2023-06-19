// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogueChoiceObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCLicked, int, ReplyIndex);

/**
 * 
 */
UCLASS(BlueprintType)
class SCOUTSODYSSEY_API UDialogueChoiceObject : public UObject
{
	GENERATED_BODY()

	
public:
	UDialogueChoiceObject();
	void SetValues(int ReplyIndexInput, const FText& ReplyInput);
	
	UPROPERTY(BlueprintReadWrite)
	int ReplyIndex;
	UPROPERTY(BlueprintReadWrite)
	FText Reply;
	UPROPERTY(BlueprintCallable)	
	FOnCLicked OnClicked;
};
