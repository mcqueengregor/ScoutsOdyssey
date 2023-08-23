// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpriteAnimationDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SCOUTSODYSSEY_API USpriteAnimationDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// The texture object containing the animation spritesheet:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UTexture* SpritesheetTexture;

	// The number of columns in the spritesheet:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 NumSpritesheetColumns;

	// The number of rows in the spritesheet:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 NumSpritesheetRows;

	// The number of empty cells in the spritesheet:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 NumEmptyFrames;

	// The intended FPS of the animation:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 PlaybackFramerate;

	// For animations that show an interaction, the frame index for when that
	// interaction should take place (e.g. frame 4 of the "hit with hammer" animation):
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 InteractionStartIndex;
};
