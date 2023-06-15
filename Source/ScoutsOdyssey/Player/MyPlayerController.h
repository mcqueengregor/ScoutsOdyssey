// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	FVector2D GetPlayerScreenCoordinate(const FVector2D Offset) const;
	FVector2D GetActorScreenCoordinate(const AActor& Actor, const FVector2D Offset) const;
	
protected:
	virtual void BeginPlay() override;

private:
};
