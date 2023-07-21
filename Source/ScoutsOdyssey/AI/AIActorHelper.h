#pragma once
#include "AnimationMap.h"
#include "ScoutsOdyssey/Animation/SpriteAnimationDataAsset.h"

template <typename T> class AIActorHelper
{
public:
	
	static void AnimationMap_SetUp(AnimationMap<T>& AnimationMap, TArray<USpriteAnimationDataAsset*>& AnimationDAs)
	{
		for (int i = 0; i < AnimationDAs.Num(); i++)
		{
			AnimationMap.Add(static_cast<T>(i), AnimationDAs[i]);
		}
	}
};
