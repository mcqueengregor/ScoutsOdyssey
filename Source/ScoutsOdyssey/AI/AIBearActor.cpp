#include "AIBearActor.h"

#include <string>

#include "AIActorHelper.h"
#include "ScoutsOdyssey/LoggingMacros.h"

void AAIBearActor::BeginPlay()
{
	Super::BeginPlay();

	AIActorHelper<FBearAnimationState>::AnimationMap_SetUp(AnimationMap, AnimationDAs);
	LOG_INT("this is the DA num!", AnimationDAs.Num());
	LOG_INT("this is the num!", AnimationMap.Num());
	CurrentAnimDetails = *AnimationMap.Find(static_cast<FBearAnimationState>(0));
	SetUpNewAnimation();
}

void AAIBearActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//LOG_FLOAT("this is the num!", AnimationMap.Num());
	
	// if(CurrentAnimDetails)
	// 	LOG_INT("Playbackframerate!", CurrentAnimDetails->PlaybackFramerate);
}

void AAIBearActor::ChangeAnimation(int Index)
{
	//CurrentAnimDetails = AnimationMap.Find(Index);
	SetUpNewAnimation();	
}
