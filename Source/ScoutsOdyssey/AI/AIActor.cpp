// Fill out your copyright notice in the Description page of Project Settings.


#include "AIActor.h"

#include "AIActorHelper.h"
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

void AAIActor::SetUpNewAnimation()
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


// Called every frame
void AAIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AnimRunningTime += DeltaTime;

	if(DynamicMaterial)
	{
		float Alpha = FMath::Clamp(AnimRunningTime/AnimDuration, 0.f, 1.f);
		
		DynamicMaterial->SetScalarParameterValue("AnimationLocalTimeNorm",
			FMath::Lerp(0, 1, Alpha));	
	}

	// LOG_FLOAT("anim duration", AnimDuration);
	// LOG_FLOAT("anim running time", AnimRunningTime);
}