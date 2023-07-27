#include "AIBearActor.h"
#include "AIActorHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AudioComponent.h"
#include "ScoutsOdyssey/LoggingMacros.h"

void AAIBearActor::BeginPlay()
{
	Super::BeginPlay();

	AIActorHelper<FBearAnimationState>::AnimationMap_SetUp(AnimationMap, AnimationDAs);
	CurrentAnimDetails = AnimationMap.Find(0);
	NewAnimation_SetUp();

	BehaviorTree_SetUp();

	// Set up audio component
	if(AIController)
	{		
		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
		if(Blackboard)
		{
			if(SFXAudioComponent)
			{
				Blackboard->SetValueAsObject("AudioComponent", SFXAudioComponent);
			}
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("BlackBoardComponent is not assigned in AIController!"));	
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller was not constructed For Dialogue Tree!"));		
	}
}

void AAIBearActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

AAIBearActor::AAIBearActor()
{
	SFXAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX AudioComponent"));
	SFXAudioComponent->SetSound(nullptr);
}

void AAIBearActor::ChangeAnimation(int Index)
{
	CurrentAnimDetails = AnimationMap.Find(Index);
	NewAnimation_SetUp();	
}
