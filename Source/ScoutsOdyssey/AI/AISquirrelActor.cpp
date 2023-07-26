﻿#include "AISquirrelActor.h"
#include "AIActorHelper.h"


void AAISquirrelActor::BeginPlay()
{
	Super::BeginPlay();

	AIActorHelper<FSquirrelAnimationState>::AnimationMap_SetUp(AnimationMap, AnimationDAs);
	LOG_INT("this is the DA num!", AnimationDAs.Num());
	LOG_INT("this is the num!", AnimationMap.Num());
	CurrentAnimDetails = AnimationMap.Find(0);
	NewAnimation_SetUp();
}

void AAISquirrelActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAISquirrelActor::ChangeAnimation(int Index)
{
	CurrentAnimDetails = AnimationMap.Find(Index);
	NewAnimation_SetUp();	
}