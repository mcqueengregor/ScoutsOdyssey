// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIAnimationActor.h"
#include "AIBeeActor.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API AAIBeeActor : public AAIAnimationActor
{
	GENERATED_BODY()

public:
	virtual void ChangeAnimation(int Index) override;
	
protected:
	virtual void BeginPlay() override;

private:
	AnimationMap<FBeeAnimationState> AnimationMap;
	
};
