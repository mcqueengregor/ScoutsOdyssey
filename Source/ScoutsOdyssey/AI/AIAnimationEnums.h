
#pragma once
#include "ScoutsOdyssey/Animation/SpriteAnimationDataAsset.h"


enum class FBearAnimationState : uint8
{
	ASLEEP = 0   UMETA(DisplayName = "Bear Asleep"),
	WAKEUP = 1    UMETA(DisplayName = "Going from asleep to Alert"),
	GOTOSLEEP = 2 UMETA(DisplayName = "Bear goes to sleep"),
	EATHONEY = 3 UMETA(DisplayName = "Bear goes to eat honey") 
};

enum class FSquirrelAnimationState : uint8
{
	IDLE = 0   UMETA(DisplayName = "Idle"),
	EATACORN = 1    UMETA(DisplayName = "Go and eat acorn"),
};

enum class FBeeAnimationState : uint8
{
	IDLE = 0,
	MOVING = 1 
};