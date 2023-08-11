// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScoutsFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API UScoutsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "Time")
	static float GetEditorTime();
};
