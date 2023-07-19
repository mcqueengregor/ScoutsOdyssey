// Fill out your copyright notice in the Description page of Project Settings.


#include "AIActor.h"

#include "AIActorHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "ScoutsOdyssey/LoggingMacros.h"

AAIActor::AAIActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}


void AAIActor::BeginPlay()
{
	Super::BeginPlay();
	CreateDynamicAnimationMaterial();
}

void AAIActor::CreateDynamicAnimationMaterial()
{
	UMaterialInterface* MaterialInterface = StaticMeshComponent->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	StaticMeshComponent->SetMaterial(0, DynamicMaterial);
}

void AAIActor::NewAnimation_SetUp()
{
	UpdateDynamicMaterialParameters();
	ResetAnimationTimes();
}

void AAIActor::UpdateDynamicMaterialParameters()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetTextureParameterValue("AnimationSpritesheet", CurrentAnimDetails->SpritesheetTexture);
		DynamicMaterial->SetScalarParameterValue("NumSpritesheetRows", CurrentAnimDetails->NumSpritesheetRows);
		DynamicMaterial->SetScalarParameterValue("NumSpritesheetColumns", CurrentAnimDetails->NumSpritesheetColumns);
	}
	else
	{
		LOG_ERROR("Dynamic Material not found");
	}
}


// goes from 0 to 1 and stays there. 
void AAIActor::ResetAnimationTimes()
{
	const int32 NumCells = CurrentAnimDetails->NumSpritesheetColumns * CurrentAnimDetails->NumSpritesheetRows;
	const int32 NumSprites = NumCells - CurrentAnimDetails->NumEmptyFrames;

	LOG_INT("num cells", NumCells);
	LOG_INT("num sprites", NumSprites);
	
	AnimDuration = static_cast<float>(NumSprites) / CurrentAnimDetails->PlaybackFramerate;

	LOG_FLOAT("anim duration", AnimDuration);
	
	AnimRunningTime = 0;
}

void AAIActor::BehaviorTree_SetUp()
{
	AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), FVector(0), FRotator(0));

	if(AIController)
	{
		AIController->RunBehaviorTree(BehaviorTree);
		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
		if(Blackboard)
		{
			// TODO: bloack board requires AI actor. which can be itself.
			// Need to be able to call change animation function, simply. 
		}
	}
}


// Called every frame
void AAIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AnimRunningTime += DeltaTime;

	if(DynamicMaterial)
	{
		float Alpha = AnimRunningTime/AnimDuration;
		if(LoopCurAnim)
		{
			Alpha = FGenericPlatformMath::Fmod(Alpha, 1);
		}
		else
		{
			Alpha = AnimRunningTime/AnimDuration;
		}
		DynamicMaterial->SetScalarParameterValue("AnimationLocalTimeNorm",
				Alpha);			
	}
}