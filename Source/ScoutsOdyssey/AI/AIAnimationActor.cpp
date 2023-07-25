// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAnimationActor.h"

#include "AIActorHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "ScoutsOdyssey/LoggingMacros.h"

AAIAnimationActor::AAIAnimationActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}


void AAIAnimationActor::BeginPlay()
{
	Super::BeginPlay();
	CreateDynamicAnimationMaterial();
}

void AAIAnimationActor::CreateDynamicAnimationMaterial()
{
	UMaterialInterface* MaterialInterface = StaticMeshComponent->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	StaticMeshComponent->SetMaterial(0, DynamicMaterial);
}

void AAIAnimationActor::NewAnimation_SetUp()
{
	UpdateDynamicMaterialParameters();
	ResetAnimationTimes();
}

void AAIAnimationActor::UpdateDynamicMaterialParameters()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetTextureParameterValue("AnimationSpritesheet", CurrentAnimDetails->SpritesheetTexture);
		DynamicMaterial->SetScalarParameterValue("NumSpritesheetRows", CurrentAnimDetails->NumSpritesheetRows);
		DynamicMaterial->SetScalarParameterValue("NumSpritesheetColumns", CurrentAnimDetails->NumSpritesheetColumns);
		DynamicMaterial->SetScalarParameterValue("AnimationLocalTimeNorm",0);			
	}
	else
	{
		LOG_ERROR("Dynamic Material not found");
	}
}


// goes from 0 to 1 and stays there. 
void AAIAnimationActor::ResetAnimationTimes()
{
	const int32 NumCells = CurrentAnimDetails->NumSpritesheetColumns * CurrentAnimDetails->NumSpritesheetRows;
	const int32 NumSprites = NumCells - CurrentAnimDetails->NumEmptyFrames;

	// LOG_INT("num cells", NumCells);
	// LOG_INT("num sprites", NumSprites);
	
	AnimDuration = static_cast<float>(NumSprites) / CurrentAnimDetails->PlaybackFramerate;

	LOG_FLOAT("anim duration", AnimDuration);
	
	AnimRunningTime = 0;
}

void AAIAnimationActor::BehaviorTree_SetUp()
{
	AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), FVector(0), FRotator(0));

	if(AIController)
	{
		AIController->RunBehaviorTree(BehaviorTree);
		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
		if(Blackboard)
		{
			Blackboard->SetValueAsObject("AIAnimationActor", this);	
		}
	}
}


// Called every frame
void AAIAnimationActor::Tick(float DeltaTime)
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
			Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
		}
		DynamicMaterial->SetScalarParameterValue("AnimationLocalTimeNorm",
				Alpha);			
	}
}