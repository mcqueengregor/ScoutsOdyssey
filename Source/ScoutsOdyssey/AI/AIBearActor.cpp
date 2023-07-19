#include "AIBearActor.h"

#include <string>

#include "AIActorHelper.h"
#include "ScoutsOdyssey/LoggingMacros.h"

void AAIBearActor::BeginPlay()
{
	Super::BeginPlay();

	AIActorHelper<FBearAnimationState>::AnimationMap_SetUp(AnimationMap, AnimationDAs);
	CurrentAnimDetails = AnimationMap.Find(0);
	NewAnimation_SetUp();

	LOG_INT("this is the DA num!", AnimationDAs.Num());
	LOG_INT("this is the num!", AnimationMap.Num());	
}

void AAIBearActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAIBearActor::ChangeAnimation(int Index)
{
	CurrentAnimDetails = AnimationMap.Find(Index);
	NewAnimation_SetUp();	
}
