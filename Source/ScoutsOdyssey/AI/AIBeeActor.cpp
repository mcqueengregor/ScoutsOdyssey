// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBeeActor.h"
#include "AIActorHelper.h"

void AAIBeeActor::ChangeAnimation(int Index)
{
	CurrentAnimDetails = AnimationMap.Find(Index);
	NewAnimation_SetUp();
}

void AAIBeeActor::BeginPlay()
{
	Super::BeginPlay();

	AIActorHelper<FBeeAnimationState>::AnimationMap_SetUp(AnimationMap, AnimationDAs);
	CurrentAnimDetails = AnimationMap.Find(0);
	NewAnimation_SetUp();
	BehaviorTree_SetUp();
	
}
