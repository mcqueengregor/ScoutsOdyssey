#include "AISquirrelActor.h"
#include "AIActorHelper.h"


void AAISquirrelActor::BeginPlay()
{
	Super::BeginPlay();

	AIActorHelper<FSquirrelAnimationState>::AnimationMap_SetUp(AnimationMap, AnimationDAs);
	CurrentAnimDetails = AnimationMap.Find(0);
	NewAnimation_SetUp();
	BehaviorTree_SetUp();	
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
