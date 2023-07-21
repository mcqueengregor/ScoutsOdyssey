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

	UCustomAnimInstance();

public:
	inline void ToggleAnimationPlayback() { bIsPlayingForwards = !bIsPlayingForwards; }

	inline bool GetIsPlayingForwards() { return bIsPlayingForwards; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controls")
	bool bIsPlayingForwards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Controls")
	bool bStartAtBeginning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation controls")
	float AnimationDuration;

	UFUNCTION(BlueprintCallable)
	static float GetSequenceDuration(UAnimSequence* Sequence) { return Sequence->GetPlayLength(); }
};
