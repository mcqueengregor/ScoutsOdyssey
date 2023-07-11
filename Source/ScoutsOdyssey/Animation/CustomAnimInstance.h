// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CustomAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API UCustomAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controls")
	bool bIsPlaybackReversed;
};
