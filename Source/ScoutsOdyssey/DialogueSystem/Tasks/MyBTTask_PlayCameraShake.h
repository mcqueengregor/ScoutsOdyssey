// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "MyBTTask_PlayCameraShake.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SCOUTSODYSSEY_API UMyBTTask_PlayCameraShake : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void OnCameraShake_Broadcast();
	
};
