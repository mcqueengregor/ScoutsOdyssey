#pragma once
#include "ScoutsOdyssey/LoggingMacros.h"
#include "ScoutsOdyssey/Animation/SpriteAnimationDataAsset.h"

template<typename T>
class AnimationMap
{
public:

	// Note that map returns a pointer. So to get the data you need to de-reference it first. 
	USpriteAnimationDataAsset* Find(T Key)
	{
		return *Map.Find(Key);
	}

	USpriteAnimationDataAsset* Find(int Index)
	{
		T Key = static_cast<T>(Index); 
		return *Map.Find(Key);
	}

	void Add(T Key, USpriteAnimationDataAsset* AnimDA)
	{
		LOG_INT("huh?", Map.Num());
		Map.Add(Key, AnimDA);
	}

	int Num()
	{
		return Map.Num();
	}

protected:
	TMap<T, USpriteAnimationDataAsset*> Map;	
	
	
};
