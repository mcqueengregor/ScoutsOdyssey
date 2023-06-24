// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

FVector2D AMyPlayerController::GetPlayerScreenCoordinate(const FVector2D Offset) const
{
	return GetActorScreenCoordinate(*GetPawn(), Offset);
}

// UMG uses viewport size as reference for widget location.
FVector2D AMyPlayerController::GetViewportSize()
{
	FVector2D Result;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(Result);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get viewport size. No GEngine."));	
	}
	return Result;
}

FVector2D AMyPlayerController::GetActorScreenCoordinate(const AActor& Actor, const FVector2D Offset) const
{
	FVector2D ScreenLocation;
	ProjectWorldLocationToScreen(Actor.GetActorLocation(), ScreenLocation);
	return ScreenLocation + Offset;	
}



