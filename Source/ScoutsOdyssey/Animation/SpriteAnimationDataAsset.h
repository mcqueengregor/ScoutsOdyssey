// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpriteAnimationDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SCOUTSODYSSEY_API USpriteAnimationDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UTexture* SpritesheetTexture;
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumSpritesheetColumns;
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumSpritesheetRows;
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumEmptyFrames;
	
	UPROPERTY(EditDefaultsOnly)
	int32 PlaybackFramerate;
};
